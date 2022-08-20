#include "./tree.h"
#include <algorithm>
#include <cassert>

namespace szn_json
{
	const tree* tree::get_child(const string& str_key_c) const
	{
		auto it = std::find_if(child_.begin(), child_.end(), [str_key_c](const pair<string, unique_ptr<tree>>& item)
		{
			return item.first == str_key_c;
		});

		if (it != child_.end())
		{
			return it->second.get();
		}

		return nullptr;
	}

	const tree* tree::get_child(const size_t n_id_c) const
	{
		if (n_id_c < child_.size())
		{
			auto it = child_.begin();
			for (size_t i = 0; i < n_id_c; ++i)
			{
				++it;
			}

			return it->second.get();
		}

		return nullptr;
	}

	const string tree::format_internal(int n_level) const
	{
		//用于控制缩进
		string str_pre;
		for (int i = 0; i < n_level; ++i)
		{
			str_pre += "\t";
		}

		string str_print;

		switch (this->value_type_)
		{
			case json_value_object:
			{
				str_print += (str_pre + "{\n");

				for (auto it = this->child_.begin(); it != child_.end(); ++it)
				{
					str_print += (str_pre + "\t\"" + it->first + "\":");

					if (it->second->value_type_ == json_value_object ||
						it->second->value_type_ == json_value_array)
					{
						str_print += ("\n" + it->second->format_internal(n_level + 1));
					}
					else
					{
						str_print += it->second->format_internal(0);
					}

					auto it_copy = it;
					if (++it_copy != child_.end())
					{
						str_print += ",\n";
					}
					else
					{
						str_print += "\n";
					}
				}

				str_print += (str_pre + "}");
				break;
			}

			case json_value_array:
			{
				str_print += (str_pre + "[\n");

				for (auto it = child_.begin(); it != child_.end(); ++it)
				{
					str_print += it->second->format_internal(n_level + 1);

					auto it_copy = it;
					if (++it_copy == child_.end())
					{
						str_print += "\n";
					}
					else
					{
						str_print += ",\n";
					}
				}

				str_print += (str_pre + "]");
				break;
			}

			case json_value_string:
			{
				str_print += (str_pre + "\"" + this->str_data_ + "\"");
				break;
			}
			case json_value_bool:
			case json_value_number:
			case json_value_null:
			{
				str_print += (str_pre + this->str_data_);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		return str_print;
	}
}