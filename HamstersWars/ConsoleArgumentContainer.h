#pragma once
#include "ConsoleArgument.h"

namespace game
{
	class ConsoleArgumentContainerException : public std::exception
	{
	public:
		ConsoleArgumentContainerException(const std::string& msg) : msg_(msg)
		{}


		char const* what() const override
		{
			return msg_.c_str();
		}

	private:
		std::string msg_;
	};

	class ConsoleArgumentContainerNotFindException : public ConsoleArgumentContainerException
	{
	public:
		explicit ConsoleArgumentContainerNotFindException(const std::string& name) :
			ConsoleArgumentContainerException(std::string("Unable to find") + name.c_str())
		{}
	};

	class ConsoleArgumentContainer
	{
	public:
		typedef std::unordered_map<std::string, ConsoleArgument*> console_argument_list;
		typedef std::vector<std::string> console_unknown_values_list;

		class ConsoleArgumentContainerIterator
		{
		public:
			typedef ConsoleArgumentContainerIterator self_type;
			typedef ConsoleArgument value_type;
			typedef value_type& reference;
			typedef value_type* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit ConsoleArgumentContainerIterator(console_argument_list::iterator iterator);

			self_type& operator++();

			reference operator*() const;
			pointer operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			console_argument_list::iterator current_;
		};

		class ConstConsoleArgumentContainerIterator
		{
		public:
			typedef ConstConsoleArgumentContainerIterator self_type;
			typedef ConsoleArgument value_type;
			typedef value_type& reference;
			typedef value_type* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef std::ptrdiff_t difference_type;

			explicit ConstConsoleArgumentContainerIterator(console_argument_list::const_iterator iterator);
			self_type& operator++();

			const reference operator*() const;
			const pointer operator->() const;

			bool operator==(const self_type& right) const;
			bool operator!=(const self_type& right) const;
		private:
			console_argument_list::const_iterator current_;
		};

		typedef ConstConsoleArgumentContainerIterator const_iterator;
		typedef ConsoleArgumentContainerIterator iterator;

		ConsoleArgumentContainer() = default;
		ConsoleArgumentContainer(const int& argc, char** argv);

		ConsoleArgumentContainer(const ConsoleArgumentContainer& other);
		ConsoleArgumentContainer& operator=(const ConsoleArgumentContainer& other);
		~ConsoleArgumentContainer();

		ConsoleArgument get_argument(const std::string& name) const;
		const console_unknown_values_list& get_unknown_values() const;

		bool contains(const std::string& arg_name) const;

		void add(const std::string& name, const std::string value);
		void add(const ConsoleArgument& argument);

		template<class Type>
		void add(const std::string& name, const Type value);

		template<>
		void add<std::string>(const std::string& name, const std::string value)
		{
			add(ConsoleArgument(name, value));
		}

		const_iterator begin() const;
		iterator begin();

		const_iterator end() const;
		iterator end();

		void clear();

		void parse_console_arguments(const int& argc, char** argv);
		ConsoleArgument operator[](const std::string& arg_name) const;

		explicit operator std::string() const;
		friend std::ostream& operator<<(std::ostream& out, const ConsoleArgumentContainer& arguments);

		std::string to_string() const;

		iterator find(const std::string& name);
		const_iterator find(const std::string& name) const;
	private:
		console_argument_list arguments_;
		console_unknown_values_list unknown_values_;

		struct Token
		{
			char type;
			std::string value;

			Token(const char& type, const std::string& value) : type(type), value(value)
			{}

			explicit Token(const std::string& value) : Token('v', value)
			{}
		};

		typedef std::vector<Token> token_list;

		static void add_specials(token_list& tokens, const std::string& str, const size_t& begin, const size_t& end);
		static token_list tokenize(const int& argc, char** argv);
		void parse(const token_list& tokens);
	};

	template <class Type>
	void ConsoleArgumentContainer::add(const std::string& name, const Type value)
	{
		add(ConsoleArgument(name, value));
	}
}

