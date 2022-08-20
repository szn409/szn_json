#ifndef __szn_json_define_h
#define __szn_json_define_h

namespace szn_json
{
	//jsonֵ������
	enum e_json_value_type
	{
		//����
		json_value_object,

		//true or false
		json_value_bool,

		//null
		json_value_null,

		//�ַ���
		json_value_string,

		//����
		json_value_number,

		//����
		json_value_array,

		//��Ч����
		json_value_invalid
	};
}

#endif //__szn_json_define_h