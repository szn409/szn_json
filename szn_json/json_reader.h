#ifndef szn_json_reader_h
#define szn_json_reader_h

#include "./tree.h"

namespace szn_json
{
	//用于解析json
	class json_reader
	{
	public:
		/*
		功能：解析json

		参数：
			@p_json_c：待解析的json字符串，必须是utf8编码
			@n_len_c：待解析的json字符串长度，若为 -1 则等价于 strlen(p_json_c)

		返回值：指示json解析是否成功
		*/
		const bool analyze(const char* p_json_c, const size_t n_len_c = -1);

		/*
		功能：获取根节点
		返回值：非空即表示有效
		*/
		const tree* get_root() const { return this->p_root_.get(); }

	private:
		//功能：读取json-value
		unique_ptr<tree> read_value();

		//功能：读取json-object
		unique_ptr<tree> read_object();

		//功能：读取json-bool
		unique_ptr<tree> read_bool();

		//功能：读取json-null
		unique_ptr<tree> read_null();

		//功能：读取json-string
		unique_ptr<tree> read_string();

		//功能：读取json-number
		unique_ptr<tree> read_number();

		//功能：读取json-array
		unique_ptr<tree> read_array();

	private:
		/*
		功能：从输入缓冲区的首字节开始偏移，直到遇见第一个非 ' ' '\r' '\n' '\t' 的字符

		返回值：指示是否成功
		*/
		const bool move_to_not_whitespace();

		//功能：获取当前输入缓冲区中的json值的类型
		e_json_value_type get_value_type();

	private:
		//标识输入缓冲区
		const char* p_begin_c_ = nullptr;
		const char* p_end_c_ = nullptr;

		//充当json的根节点
		unique_ptr<tree> p_root_;
	};
}

#endif //szn_json_reader_h



//调试 没完成去重