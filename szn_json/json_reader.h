#ifndef szn_json_reader_h
#define szn_json_reader_h

#include "./tree.h"

namespace szn_json
{
	//���ڽ���json
	class json_reader
	{
	public:
		/*
		���ܣ�����json

		������
			@p_json_c����������json�ַ�����������utf8����
			@n_len_c����������json�ַ������ȣ���Ϊ -1 ��ȼ��� strlen(p_json_c)

		����ֵ��ָʾjson�����Ƿ�ɹ�
		*/
		const bool analyze(const char* p_json_c, const size_t n_len_c = -1);

		/*
		���ܣ���ȡ���ڵ�
		����ֵ���ǿռ���ʾ��Ч
		*/
		const tree* get_root() const { return this->p_root_.get(); }

	private:
		//���ܣ���ȡjson-value
		unique_ptr<tree> read_value();

		//���ܣ���ȡjson-object
		unique_ptr<tree> read_object();

		//���ܣ���ȡjson-bool
		unique_ptr<tree> read_bool();

		//���ܣ���ȡjson-null
		unique_ptr<tree> read_null();

		//���ܣ���ȡjson-string
		unique_ptr<tree> read_string();

		//���ܣ���ȡjson-number
		unique_ptr<tree> read_number();

		//���ܣ���ȡjson-array
		unique_ptr<tree> read_array();

	private:
		/*
		���ܣ������뻺���������ֽڿ�ʼƫ�ƣ�ֱ��������һ���� ' ' '\r' '\n' '\t' ���ַ�

		����ֵ��ָʾ�Ƿ�ɹ�
		*/
		const bool move_to_not_whitespace();

		//���ܣ���ȡ��ǰ���뻺�����е�jsonֵ������
		e_json_value_type get_value_type();

	private:
		//��ʶ���뻺����
		const char* p_begin_c_ = nullptr;
		const char* p_end_c_ = nullptr;

		//�䵱json�ĸ��ڵ�
		unique_ptr<tree> p_root_;
	};
}

#endif //szn_json_reader_h



//���� û���ȥ��