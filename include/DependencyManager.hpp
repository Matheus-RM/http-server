#pragma once

#include <unordered_map>
#include <string>
#include <any>

#include "Message.hpp"

class DependencyManager
{
	public:
		DependencyManager() {}
		virtual ~DependencyManager() {}

	public:

		template <class T, class... Args>
		void addDependency(Args... args)
		{
			auto dependency = std::make_any<T>(args...);
			mDependencies.emplace(typeid(T).name(), dependency);
		}

		template <class T>
		std::pair<T, bool> getDependency()
		{
			const auto&& id = std::string(typeid(T).name());
			auto it = mDependencies.find(id);

			MESSAGE("\t==> Getting dependency ", id, "\n");

			if(it == mDependencies.end())
			{
				MESSAGE("\t==> Dependency not found", "\n");
				return std::make_pair(T(), false);
			}

			return std::make_pair(std::any_cast<T>(it->second), true);
		}

	private:
		std::unordered_map<std::string, std::any> mDependencies;
};