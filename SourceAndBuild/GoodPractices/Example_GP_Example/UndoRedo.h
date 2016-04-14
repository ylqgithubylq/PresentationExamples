#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <array>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>

namespace UndoRedo
{
	struct ID
	{
		size_t id;
	};
	bool operator==(ID l, ID r)
	{
		return l.id == r.id;
	}
}

namespace std
{
	template<>
	struct std::hash<UndoRedo::ID>
	{
		size_t operator()(UndoRedo::ID id) const
		{
			return hash<size_t>()(id.id);
		}
	};
}

namespace UndoRedo
{
	using namespace std;

	struct IResource
	{
		IResource(ID id)
			: id(id)
		{
		}
		virtual ~IResource() = default;


		ID GetID() const
		{
			return id;
		}

	private:
		ID id;
	};

	class ResourceManager
	{
		struct Deleter
		{
			ResourceManager* manager;
			Deleter(ResourceManager* manager)
				: manager(manager)
			{
			}

			void operator()(IResource* p)
			{
				if (p != nullptr)
				{
					manager->resources.erase(p->GetID());
					delete p;
				}
			}
		};

	public:
		template <typename T>
		shared_ptr<T> GetResource(ID id)
		{
			auto found = resources.find(id);
			if (found != resources.end())
			{
				return dynamic_pointer_cast<T>(found->second.lock());
			}
			else
			{
				shared_ptr<T> created(new T(id), Deleter(this));
				resources.insert(make_pair(id, weak_ptr<T>(created)));
				return created;
			}
		}

		~ResourceManager()
		{
			if (resources.size() > 0)
			{
				cout << "resource leaks!" << endl;
			}
		}

	private:


		unordered_map<ID, weak_ptr<IResource>> resources;
	};

	struct IUndoCommand
	{
		virtual ~IUndoCommand() = default;
		virtual void Undo() = 0;
		virtual void Redo() = 0;
	};

	struct UndoManager
	{
		void AddUndoCommand(unique_ptr<IUndoCommand> command)
		{
			redos.clear();
			undoCommands.push_back(move(command));
		}
		bool Undo()
		{
			if (!undoCommands.empty())
			{
				auto last = move(undoCommands.back());
				undoCommands.pop_back();
				last->Undo();
				redos.push_back(move(last));
				return true;
			}
			else
			{
				return false;
			}
		}
		bool Redo()
		{
			if (!redos.empty())
			{
				auto last = move(redos.back());
				redos.pop_back();
				last->Redo();
				undoCommands.push_back(move(last));
				return true;
			}
			else
			{
				return false;
			}
		}
		vector<unique_ptr<IUndoCommand>> undoCommands;
		vector<unique_ptr<IUndoCommand>> redos;
	};




	struct ResourceA : IResource
	{
		using IResource::IResource;
	};

	struct ResourceB : IResource
	{
		using IResource::IResource;


		void SetA(size_t index, shared_ptr<ResourceA> a)
		{
			assert(index < as.size());
			as[index] = move(a);
		}

		size_t GetACount() const
		{
			return as.size();
		}

		vector<shared_ptr<ResourceA>> const& GetAS() const
		{
			return as;
		}
		void SetAS(vector<shared_ptr<ResourceA>> const& as)
		{
			this->as = as;
		}
		vector<shared_ptr<ResourceA>> as;
	};

	struct ASChange : IUndoCommand
	{
		shared_ptr<ResourceB> b;
		vector<shared_ptr<ResourceA>> oldAs;
		vector<shared_ptr<ResourceA>> newAs;
		ASChange(shared_ptr<ResourceB> b, vector<shared_ptr<ResourceA>> oldAs, vector<shared_ptr<ResourceA>> newAs)
			: b(move(b)), oldAs(move(oldAs)), newAs(move(newAs))
		{
		}

		virtual void Undo() override
		{
			b->SetAS(oldAs);
		}

		virtual void Redo() override
		{
			b->SetAS(newAs);
		}

	};




	ResourceManager resourceManager;
	UndoManager undoManager;

	struct BBuilder
	{
		shared_ptr<ResourceB> b;

		BBuilder(shared_ptr<ResourceB> b)
			: b(move(b))
		{
		}

		bool InitializeB()
		{
			vector<shared_ptr<ResourceA>> as;
			for (size_t i = 0; i < 5; i++)
			{
				as.push_back(resourceManager.GetResource<ResourceA>({ i }));
			}

			auto undo = make_unique<ASChange>(b, b->GetAS(), move(as));
			undo->Redo();
			undoManager.AddUndoCommand(move(undo));

			return true;
		}

		bool SetA(size_t index, ID newID)
		{
			if (index >= b->GetACount())
			{
				return false;
			}
			auto as = b->GetAS();
			as[index] = resourceManager.GetResource<ResourceA>(newID);

			auto undo = make_unique<ASChange>(b, b->GetAS(), move(as));
			undo->Redo();
			undoManager.AddUndoCommand(move(undo));

			return true;
		}

		bool AddA(ID newID)
		{
			auto as = b->GetAS();
			as.push_back(resourceManager.GetResource<ResourceA>(newID));

			auto undo = make_unique<ASChange>(b, b->GetAS(), move(as));
			undo->Redo();
			undoManager.AddUndoCommand(move(undo));

			return true;
		}

		bool RemoveA()
		{
			auto as = b->GetAS();
			as.pop_back();

			auto undo = make_unique<ASChange>(b, b->GetAS(), move(as));
			undo->Redo();
			undoManager.AddUndoCommand(move(undo));

			return true;
		}
	};




	void Run()
	{
		BBuilder builder(resourceManager.GetResource<ResourceB>({ 1300 }));

		builder.InitializeB();		// 0, 1, 2, 3, 4
		builder.SetA(2, { 12 });	// 0, 1, 12, 3, 4
		builder.AddA({ 15 });		// 0, 1, 12, 3, 4, 15
		undoManager.Undo();			// 0, 1, 12, 3, 4
		undoManager.Undo();			// 0, 1, 2, 3, 4
		undoManager.Redo();			// 0, 1, 12, 3, 4
		builder.RemoveA();			// 0, 1, 12, 3
		auto as = builder.b->GetAS();
		as = as;
	}
}
