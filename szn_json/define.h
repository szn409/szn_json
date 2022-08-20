#ifndef __szn_json_define_h
#define __szn_json_define_h

namespace szn_json
{
	//json值的类型
	enum e_json_value_type
	{
		//对象
		json_value_object,

		//true or false
		json_value_bool,

		//null
		json_value_null,

		//字符串
		json_value_string,

		//数字
		json_value_number,

		//数组
		json_value_array,

		//无效类型
		json_value_invalid
	};
}

#endif //__szn_json_define_h