#include "./szn_json/json_reader.h"

const char* p_json_c =
"{\"hello\" : \"world\", \"a\" : [1, null, false, true, [ {\"1\" : 2, \"3\" : 4} ], {}, [[]], 3.14]}";

int main()
{
	szn_json::json_reader szn;
	auto re = szn.analyze(p_json_c);
	//re = true

	auto p_root = szn.get_root();
	std::string str = p_root->format().c_str();
	printf("%s\n", str.c_str());
	/*
	{
		"hello":"world",
		"a":
		[
			1,
			null,
			false,
			true,
			[
				{
					"1":2,
					"3":4
				}
			],
			{
			},
			[
				[
				]
			],
			3.14
		]
	}
	*/

 	auto v0 = p_root->get_child("hello");
	//v0 = 0x000001a72583e890 {str_data_="world" child_={ size=0 } value_type_=json_value_string (3) }

	auto v0_value = v0->get_value();
	//v0_value = "world"

	auto v0_type = v0->get_type();
	//v0_type = json_value_string (3)


 	auto v1 = p_root->get_child(1);
	//v1 = 0x000001be93a7e7c0 {str_data_="" child_={ size=8 } value_type_=json_value_array (5) }

	auto v2 = v1->get_child(0);
	//v2 = 0x000001a950eaede0 {str_data_="1" child_={ size=0 } value_type_=json_value_number (4) }

	system("pause");
    return 0;
}