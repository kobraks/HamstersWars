#include "stdafx.h"
#include "ConsoleArgumentContainer.h"


namespace game
{
	ConsoleArgumentContainer::ConsoleArgumentContainerIterator::ConsoleArgumentContainerIterator(
		console_argument_list::iterator iterator) : current_(iterator)
	{
	}

	ConsoleArgumentContainer::ConsoleArgumentContainerIterator::self_type& ConsoleArgumentContainer::
		ConsoleArgumentContainerIterator::operator++()
	{
		++current_;
		return *this;
	}

	ConsoleArgumentContainer::ConsoleArgumentContainerIterator::reference ConsoleArgumentContainer::
		ConsoleArgumentContainerIterator::operator*() const
	{
		return *current_->second;
	}

	ConsoleArgumentContainer::ConsoleArgumentContainerIterator::pointer ConsoleArgumentContainer::
		ConsoleArgumentContainerIterator::operator->() const
	{
		return current_->second;
	}

	bool ConsoleArgumentContainer::ConsoleArgumentContainerIterator::operator==(const self_type& right) const
	{
		return right.current_ == current_;
	}

	bool ConsoleArgumentContainer::ConsoleArgumentContainerIterator::operator!=(const self_type& right) const
	{
		return right.current_ != current_;
	}

	ConsoleArgumentContainer::ConstConsoleArgumentContainerIterator::ConstConsoleArgumentContainerIterator(
		console_argument_list::const_iterator iterator) : current_(iterator)
	{
	}

	ConsoleArgumentContainer::ConstConsoleArgumentContainerIterator::self_type& ConsoleArgumentContainer::
		ConstConsoleArgumentContainerIterator::operator++()
	{
		++current_;
		return *this;
	}

	ConsoleArgumentContainer::ConstConsoleArgumentContainerIterator::reference ConsoleArgumentContainer::
		ConstConsoleArgumentContainerIterator::operator*() const
	{
		return *current_->second;
	}

	const ConsoleArgumentContainer::ConstConsoleArgumentContainerIterator::pointer ConsoleArgumentContainer::
		ConstConsoleArgumentContainerIterator::operator->() const
	{
		return current_->second;
	}

	bool ConsoleArgumentContainer::ConstConsoleArgumentContainerIterator::operator==(const self_type& right) const
	{
		return right.current_ == current_;
	}

	bool ConsoleArgumentContainer::ConstConsoleArgumentContainerIterator::operator!=(const self_type& right) const
	{
		return right.current_ != current_;
	}

	ConsoleArgumentContainer::ConsoleArgumentContainer(const int& argc, char** argv)
	{
		parse_console_arguments(argc, argv);
	}

	ConsoleArgumentContainer::ConsoleArgumentContainer(const ConsoleArgumentContainer& other)
	{
		clear();
		unknown_values_ = other.unknown_values_;

		for (auto pair : other.arguments_)
			add(*pair.second);
	}

	ConsoleArgumentContainer& ConsoleArgumentContainer::operator=(const ConsoleArgumentContainer& other)
	{
		clear();
		unknown_values_ = other.unknown_values_;

		for (auto pair : other.arguments_)
			add(*pair.second);

		return *this;
	}

	ConsoleArgumentContainer::~ConsoleArgumentContainer()
	{
		clear();
	}

	ConsoleArgument ConsoleArgumentContainer::get_argument(const std::string& name) const
	{
		const auto iterator = arguments_.find(utils::to_upper_copy(utils::trim_copy(name)));
		if (iterator != arguments_.end())
			return *iterator->second;

		throw ConsoleArgumentContainerNotFindException(name);
	}

	const ConsoleArgumentContainer::console_unknown_values_list& ConsoleArgumentContainer::get_unknown_values() const
	{
		return unknown_values_;
	}

	bool ConsoleArgumentContainer::contains(const std::string& arg_name) const
	{
		return arguments_.find(utils::to_upper_copy(utils::trim_copy(arg_name))) != arguments_.end();
	}

	void ConsoleArgumentContainer::add(const std::string& name, const std::string value)
	{
		add(ConsoleArgument(name, value));
	}

	void ConsoleArgumentContainer::add(const ConsoleArgument& argument)
	{
		const auto name = utils::trim_copy(argument.name());

		if (!contains(argument.name()))
		{
			LOG(LOG_DEBUG, "Adding argument<key: %s> (name: %s; value: %s)", utils::to_upper_copy(name).c_str(), name.c_str(),
				utils::trim_copy(argument.value<std::string>()).c_str());

			arguments_[utils::to_upper_copy(name)] = new ConsoleArgument(
				name, utils::trim_copy(argument.value<std::string>()));
		}
	}

	ConsoleArgumentContainer::const_iterator ConsoleArgumentContainer::begin() const
	{
		return ConstConsoleArgumentContainerIterator(arguments_.begin());
	}

	ConsoleArgumentContainer::iterator ConsoleArgumentContainer::begin()
	{
		return ConsoleArgumentContainerIterator(arguments_.begin());
	}

	ConsoleArgumentContainer::const_iterator ConsoleArgumentContainer::end() const
	{
		return ConstConsoleArgumentContainerIterator(arguments_.end());
	}

	ConsoleArgumentContainer::iterator ConsoleArgumentContainer::end()
	{
		return ConsoleArgumentContainerIterator(arguments_.end());
	}

	void ConsoleArgumentContainer::clear()
	{
		for (auto pair : arguments_)
			delete pair.second;

		arguments_.clear();
		unknown_values_.clear();
	}

	void ConsoleArgumentContainer::add_specials(token_list& tokens, const std::string& str, const size_t& begin,
		const size_t& end)
	{
		tokens.push_back(Token('a', utils::trim_copy(str.substr(begin, end - begin))));
		tokens.push_back(Token(utils::trim_copy(str.substr(end + 1))));
	}

	ConsoleArgumentContainer::token_list ConsoleArgumentContainer::tokenize(const int& argc, char** argv)
	{
		token_list tokens;
		tokens.reserve(argc);

		for (size_t i = 1; argc > i; ++i)
		{
			auto str = utils::trim_copy(argv[i]);

			if (str == "=")
			{
				tokens[tokens.size() - 1].type = 'a';
				continue;
			}

			const auto found = str.find_last_of("/-");
			const auto special = str.find_last_of('=');

			if (found != std::string::npos)
			{
				if (special != std::string::npos)
				{
					add_specials(tokens, str, found + 1, special);
				}
				else
					tokens.push_back(Token('a', str.substr(found + 1)));
			}
			else if (special != std::string::npos)
			{
				add_specials(tokens, str, 0, special);
			}
			else
				tokens.push_back(Token(str));
		}

		return tokens;
	}

	void ConsoleArgumentContainer::parse(const token_list& tokens)
	{
		for (auto token = tokens.begin(); token != tokens.end(); ++token)
		{
			if (token->type == 'v')
			{
				LOG(LOG_DEBUG, "Adding unknow value: %s", token->value.c_str())
				unknown_values_.push_back(utils::trim_copy(token->value));
			}
			else
			{
				const auto name = token->value;
				auto next_token = token;
				++next_token;

				if (next_token != tokens.end() && next_token->type == 'v')
					add(ConsoleArgument(name, (++token)->value));
				else
					add(ConsoleArgument(name));
			}
		}
	}

	void ConsoleArgumentContainer::parse_console_arguments(const int& argc, char** argv)
	{
		std::string path = argv[0];

		const auto found = path.find_last_of("/\\");
		auto name = path.substr(found + 1);
		path = path.substr(0, found);

		unknown_values_.reserve(argc - 1);
		arguments_.reserve(argc + 2);

		add("program_name", name);
		add("program_path", path);
		add("program", std::string(argv[0]));

		parse(tokenize(argc, argv));
	}

	ConsoleArgument ConsoleArgumentContainer::operator[](const std::string& arg_name) const
	{
		return get_argument(arg_name);
	}

	ConsoleArgumentContainer::operator std::basic_string<char>() const
	{
		return to_string();
	}

	std::string ConsoleArgumentContainer::to_string() const
	{
		std::stringstream string;
		if (contains("program"))
			string << get_argument("program") << " ";

		size_t i = 0;
		for (auto pair : arguments_)
		{
			if (i < 3)
				continue;

			i++;

			auto argument = *pair.second;

			string << argument;
			string << "=";
			string << argument;
			string << " ";
		}

		for (auto value : unknown_values_)
		{
			string << value;
			string << " ";
		}

		return utils::trim_copy(string.str());
	}

	ConsoleArgumentContainer::iterator ConsoleArgumentContainer::find(const std::string& name)
	{
		return ConsoleArgumentContainerIterator(arguments_.find(utils::to_upper_copy(utils::trim_copy(name))));
	}

	ConsoleArgumentContainer::const_iterator ConsoleArgumentContainer::find(const std::string& name) const
	{
		return ConstConsoleArgumentContainerIterator(arguments_.find(utils::to_upper_copy(utils::trim_copy(name))));
	}

	std::ostream& operator<<(std::ostream& out, const ConsoleArgumentContainer& arguments)
	{
		out << arguments.to_string();
		return out;
	}
}