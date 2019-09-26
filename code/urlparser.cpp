#include "urlparser.h"

namespace urlparser
{
	const std::string Parser::sm_urlRegex = { "(https?)://[^\\s]*" };

	const std::string Parser::sm_domainRegex = { "//[a-zA-Z0-9.-]*" };

	const std::string Parser::sm_pathRegex = { "[a-zA-Z0-9,/+_]*\\.?[a-zA-Z0-9,/+_]*" };

	Parser::Parser(void) = default;

	Parser::Parser(const Parser &other) = default;

	Parser::~Parser(void) = default;

	Parser& Parser::operator=(const Parser &rhs) = default;

	bool Parser::empty()
	{
		return (this->m_TotalURLs && this->m_TopDomains.size()
			&& this->m_TopPaths.size());
	}

	std::size_t Parser::countURL()
	{
		return this->m_TotalURLs;
	}

	void Parser::clear()
	{
		this->m_TotalURLs = 0;
		this->m_TopDomains.clear();
		this->m_TopPaths.clear();
	}

	void Parser::read(const std::string &data)
	{
		auto fn_processURLPath = [this](auto url)
		{
			return this->processURLPath(url);
		};
		this->search(data, this->sm_urlRegex, fn_processURLPath);
	}

	void Parser::read(std::istream &is)
	{
		std::string buffer{};
		while (std::getline(is, buffer))
		{
			this->read(buffer);
		}
	}

	std::ostream& Parser::write(std::ostream &os)
	{
		os << "total urls " << this->m_TotalURLs
			<< ", domains " << this->m_TopDomains.size()
			<< ", paths " << this->m_TopPaths.size()
			<< std::endl;
		os << std::endl;
		os << "top domains" << std::endl;
		for (auto it : m_TopDomains)
		{
			os << it.second << ' ' << it.first << std::endl;
		}
		os << std::endl;
		os << "top paths" << std::endl;
		for (auto it : m_TopPaths)
		{
			os << it.second << ' ' << it.first << std::endl;
		}
		return os;
	}

	bool Parser::processURLPath(const std::string &url)
	{
		// found url
		++this->m_TotalURLs;
		// get domain
		std::string domain{};
		this->search(url, this->sm_domainRegex,
			[&domain](const std::string res)
			{
				domain = res.substr(2);
				return true;
			}
		);

		auto offset = url.find(domain) + domain.size();
		auto maybe_path = url.substr(offset);
		// get path
		std::string path{};
		this->search(maybe_path, this->sm_pathRegex,
			[&path](const std::string res)
			{
				path = res;
				return true;
			}
		);
		if (path.empty())
		{
			path.push_back('/');
		}
		++this->m_TopDomains[domain];
		++this->m_TopPaths[path];

		return false;
	}

	template<typename Callback>
	void Parser::search(const std::string &str, const std::string &pattern,
		const Callback &callback)
	{
		const std::regex reg{ pattern };
		for (std::sregex_iterator it{ str.cbegin(), str.cend(), reg }, end{};
			it != end; ++it)
		{
			if (callback(it->str()))
			{
				return;
			}
		}
	}
}