#ifndef __szn_json_tree_h
#define __szn_json_tree_h
#include "./define.h"
#include <string>
#include <memory>
#include <list>

namespace szn_json
{
	using std::string;
	using std::list;
	using std::pair;
	using std::unique_ptr;

	class json_reader;

	//存储json的树形结构
	class tree
	{
		friend json_reader;

	private:
		//json value 的 data
		string str_data_;

		//json value的子项
		list<pair<string/*json-key*/, unique_ptr<tree>>> child_;

		//json值的类型
		e_json_value_type value_type_ = json_value_invalid;

	private:
		//功能：获取格式对齐的结果
		const string format_internal(int n_level = 0) const;

	public:
		/*
		功能：根据key名称，获取子项

		形参：
			@str_key_c：key名称

		返回值：非空即表示成功
		*/
		const tree* get_child(const string& str_key_c) const;

		/*
		功能：根据子项下标，获取子项

		形参：
			@n_id_c：子项下标

		返回值：非空即成功
		*/
		const tree* get_child(const size_t n_id_c) const;

		
		//功能：获取当前项的值，可自行根据项类型进行相应转换
		const string get_value() const { return this->str_data_; }

		//功能：获取当前项的类型
		const e_json_value_type get_type() const { return this->value_type_; }

		//功能：获取格式对齐的结果
		const string format() const { return this->format_internal(); }
	};
}

#endif //__szn_json_tree_h