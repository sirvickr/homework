#pragma once

#include "Coffee.h"

namespace CppWinFormApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnMake;
	protected:

	private: System::Windows::Forms::TextBox^  txtSum;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnMake = (gcnew System::Windows::Forms::Button());
			this->txtSum = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// btnMake
			// 
			this->btnMake->Location = System::Drawing::Point(77, 42);
			this->btnMake->Name = L"btnMake";
			this->btnMake->Size = System::Drawing::Size(109, 32);
			this->btnMake->TabIndex = 0;
			this->btnMake->Text = L"Приготовить кофе";
			this->btnMake->UseVisualStyleBackColor = true;
			this->btnMake->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// txtSum
			// 
			this->txtSum->Location = System::Drawing::Point(33, 90);
			this->txtSum->Name = L"txtSum";
			this->txtSum->Size = System::Drawing::Size(153, 20);
			this->txtSum->TabIndex = 1;
			this->txtSum->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(396, 314);
			this->Controls->Add(this->txtSum);
			this->Controls->Add(this->btnMake);
			this->Name = L"MyForm";
			this->Text = L"Автомат по продаже кофе";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			txtSum->Text = "asdf";
		}
		private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		}
		private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
			;
		}
	private:
		array<Coffee^>^ coffee;
		//Coffee^ coffee;
	};
}
