#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <exception>
#include <functional>
#include <regex>

namespace urlparser
{
	class Parser final
	{
	public:
// Constructor & Destructor
		Parser(void);
		Parser(const Parser&);
		~Parser(void);
// Operator
		Parser& operator=(const Parser&);
// Public methods
		bool empty();
		std::size_t countURL();
		void clear();
		void read(const std::string&);
		void read(std::istream&);
		std::ostream& write(std::ostream&);
	private:
		// Members
		std::size_t m_TotalURLs;
		std::map<std::string, std::size_t> m_TopDomains;
		std::map<std::string, std::size_t> m_TopPaths;
		// Static members
		static const std::string sm_urlRegex;
		static const std::string sm_domainRegex;
		static const std::string sm_pathRegex;
		// Private methods
		bool processURLPath(const std::string &url);
		template<typename Callback>
		void search(const std::string &str, const std::string &pattern,
			const Callback &callback);
	};
}