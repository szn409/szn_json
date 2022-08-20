#include "./json_reader.h"
#include <cassert>

namespace szn_json
{
	const bool json_reader::analyze(const char* p_json_c, const size_t n_len_c /* = -1 */)
	{
		assert(p_json_c);

		this->p_begin_c_ = p_json_c;
		this->p_end_c_ = p_begin_c_ + (-1 == n_len_c ? strlen(p_json_c) : n_len_c);

		p_root_ = this->read_value();
		if (!p_root_)
		{
			return false;
		}
		
		//json末尾判断
		if (this->move_to_not_whitespace())
		{
			p_root_ = nullptr;
			return false;
		}

		return true;
	}

	unique_ptr<tree> json_reader::read_value()
	{
		if (!this->move_to_not_whitespace())
		{
			return nullptr;
		}

		switch (this->get_value_type())
		{
			case json_value_object: return this->read_object();
			case json_value_bool: return this->read_bool();
			case json_value_null: return this->read_null();
			case json_value_string: return this->read_string();
 			case json_value_number: return this->read_number();
 			case json_value_array: return this->read_array();
 			case json_value_invalid: break;
 			default:
			{
 				assert(false);
				break;
 			}
		}

		return nullptr;
	}

	unique_ptr<tree> json_reader::read_object()
	{
		if (!this->move_to_not_whitespace())
		{
			return nullptr;
		}

		assert(*p_begin_c_ == '{');

		//偏移 object 的 {
		++p_begin_c_;

		unique_ptr<tree> p_now_node(new tree);
		p_now_node->value_type_ = json_value_object;

		unique_ptr<tree> json_key;
		unique_ptr<tree> json_value;

		while (p_begin_c_ != p_end_c_)
		{
			if (!json_key)
			{
				//空项判断
				do 
				{
					if (!this->move_to_not_whitespace())
					{
						return nullptr;
					}

					if (*p_begin_c_ == '}')
					{
						++p_begin_c_;
						return p_now_node;
					}
				} while (false);

				//获取 json-key
				json_key = this->read_string();
				if (!json_key)
				{
					return nullptr;
				}
			}
			else
			{
				//过滤key 与 value 之间的 :
				if (!this->move_to_not_whitespace() || *p_begin_c_++ != ':')
				{
					return nullptr;
				}

				//获取 json-value
				json_value = this->read_value();
				if (!json_value)
				{
					return nullptr;
				}

				//成功获取一项
				p_now_node->child_.emplace_back(std::make_pair(json_key->str_data_, std::move(json_value)));
				json_key = nullptr;

				if (!move_to_not_whitespace())
				{
					return nullptr;
				}

				if (*p_begin_c_ == ',')
				{
					//过滤项之间的逗号
					++p_begin_c_;
				}
				else if (*p_begin_c_ == '}')
				{
					//读取结束
					++p_begin_c_;
					break;
				}
			}
		}

		return p_now_node;
	}

	unique_ptr<tree> json_reader::read_bool()
	{
		if (!this->move_to_not_whitespace())
		{
			return nullptr;
		}

		assert(*p_begin_c_ == 't' || *p_begin_c_ == 'f');

		unique_ptr<tree> p_node(new tree);
		p_node->value_type_ = json_value_bool;

		int n_copy_len = 0;

		if (*p_begin_c_ == 't')
		{
			n_copy_len = 4;
		}
		else if (*p_begin_c_ == 'f')
		{
			n_copy_len = 5;
		}

		if (p_end_c_ - p_begin_c_ < n_copy_len)
		{
			return nullptr;
		}

		p_node->str_data_ = string(p_begin_c_, n_copy_len);
		p_begin_c_ += n_copy_len;

		return p_node;
	}

	unique_ptr<tree> json_reader::read_null()
	{
		if (!this->move_to_not_whitespace())
		{
			return nullptr;
		}

		assert(*p_begin_c_ == 'n');

		unique_ptr<tree> p_node(new tree);
		p_node->value_type_ = json_value_null;

		int n_copy_len = 4;

		if (p_end_c_ - p_begin_c_ < n_copy_len)
		{
			return false;
		}

		p_node->str_data_ = string(p_begin_c_, n_copy_len);
		p_begin_c_ += n_copy_len;

		return p_node;
	}

	unique_ptr<tree> json_reader::read_string()
	{
		if (!this->move_to_not_whitespace() || *p_begin_c_ != '"')
		{
			return nullptr;
		}

		unique_ptr<tree> p_node(new tree);

		p_node->value_type_ = json_value_string;

		auto p_start = p_begin_c_;
		auto p_end_c = p_end_c_;

		while (p_start < p_end_c)
		{
			if (*++p_begin_c_ == '"')
			{
				p_node->str_data_ = string(p_start + 1, p_begin_c_);

				//偏移字符串末尾的 "
				assert(*p_begin_c_ == '"');
				++p_begin_c_;

				return p_node;
			}
		}

		return nullptr;
	}

	unique_ptr<tree> json_reader::read_number()
	{
		if (!this->move_to_not_whitespace())
		{
			return nullptr;
		}

		assert(isalnum(*p_begin_c_) || *p_begin_c_ == '-');

		unique_ptr<tree> p_node(new tree);
		p_node->value_type_ = json_value_number;

		auto p_start_c = p_begin_c_;
		char n_value = 0;

		while (p_begin_c_ < p_end_c_)
		{
			n_value = *++p_begin_c_;

			if (!isalnum(n_value) && n_value != '.')
			{
				break;
			}
		}

		p_node->str_data_ = string(p_start_c, p_begin_c_);
		if (p_node->str_data_ == "-" ||
			p_node->str_data_.back() == '.' ||
			std::count(p_node->str_data_.begin(), p_node->str_data_.end(), '.') > 1)
		{
			return nullptr;
		}

		return p_node;
	}

	unique_ptr<tree> json_reader::read_array()
	{
		if (!this->move_to_not_whitespace())
		{
			return nullptr;
		}

		unique_ptr<tree> p_node(new tree);
		p_node->value_type_ = json_value_array;

		assert(*p_begin_c_ == '[');
		++p_begin_c_;

		while (p_begin_c_ < p_end_c_)
		{
			//空项判断
			do
			{
				if (!this->move_to_not_whitespace())
				{
					return nullptr;
				}

				if (*p_begin_c_ == ']')
				{
					++p_begin_c_;
					return p_node;
				}
			} while (false);

			auto value = this->read_value();
			if (!value)
			{
				return nullptr;
			}

			if (value->value_type_ == json_value_string)
			{
				if (!this->move_to_not_whitespace())
				{
					return nullptr;
				}

				if (*p_begin_c_ == ':')
				{
					//当前value不是字符串，而是object
					value->value_type_ = json_value_object;

					//偏移封号
					++p_begin_c_; 

					value->child_.emplace_back(std::make_pair(value->str_data_, this->read_value()));
					value->str_data_.clear();
				}
			}

			p_node->child_.emplace_back(std::make_pair("", std::move(value)));


			if (!this->move_to_not_whitespace())
			{
				return nullptr;
			}

			if (*p_begin_c_ != ',' && *p_begin_c_ != ']')
			{
				return nullptr;
			}

			if (*p_begin_c_ == ']')
			{
				break;
			}
			else if (*p_begin_c_ == ',')
			{
				//调试 此处需要判断下 和 object 一样的 多个,的问题
				++p_begin_c_;
				continue;
			}
		}

		//偏移数组最后一个 ]
		++p_begin_c_;
		
		return p_node;
	}

	const bool json_reader::move_to_not_whitespace()
	{
		char n_value = 0;

		while (p_begin_c_ < p_end_c_)
		{
			n_value = *p_begin_c_++;

			if (n_value != ' ' && n_value != '\r' && n_value != '\n' && n_value != '\t')
			{
				--p_begin_c_;
				return true;
			}
		}

		return false;
	}

	e_json_value_type json_reader::get_value_type()
	{
		char n_value_c = *p_begin_c_;

		switch (n_value_c)
		{
			case '{': return json_value_object;

			case 'f':
			case 't': return json_value_bool;

			case 'n': return json_value_null;
			case '"': return json_value_string;
			case '[': return json_value_array;

			default:
			{
				if (isdigit(n_value_c) || '-' == n_value_c)
				{
					return json_value_number;
				}
			}
		}
		return json_value_invalid;
	}
}