#pragma once

#include <vector>
#include <optional>

#include <crepuscule/Config.hpp>
#include <crepuscule/Token.hpp>

namespace crepuscule
{

class Tokenizer
{
	public:
		Tokenizer(Config conf);

		Expression operator()(std::string_view input);

	private:
		void prepare_config();
		bool is_config_prepared() const;

	private:
		Config _config;
};

} // namespace crepuscule