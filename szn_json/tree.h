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

	//�洢json�����νṹ
	class tree
	{
		friend json_reader;

	private:
		//json value �� data
		string str_data_;

		//json value������
		list<pair<string/*json-key*/, unique_ptr<tree>>> child_;

		//jsonֵ������
		e_json_value_type value_type_ = json_value_invalid;

	private:
		//���ܣ���ȡ��ʽ����Ľ��
		const string format_internal(int n_level = 0) const;

	public:
		/*
		���ܣ�����key���ƣ���ȡ����

		�βΣ�
			@str_key_c��key����

		����ֵ���ǿռ���ʾ�ɹ�
		*/
		const tree* get_child(const string& str_key_c) const;

		/*
		���ܣ����������±꣬��ȡ����

		�βΣ�
			@n_id_c�������±�

		����ֵ���ǿռ��ɹ�
		*/
		const tree* get_child(const size_t n_id_c) const;

		
		//���ܣ���ȡ��ǰ���ֵ�������и��������ͽ�����Ӧת��
		const string get_value() const { return this->str_data_; }

		//���ܣ���ȡ��ǰ�������
		const e_json_value_type get_type() const { return this->value_type_; }

		//���ܣ���ȡ��ʽ����Ľ��
		const string format() const { return this->format_internal(); }
	};
}

#endif //__szn_json_tree_h