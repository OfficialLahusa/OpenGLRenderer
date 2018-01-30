#pragma once
#include <iostream>
#include <vector>

class GameObject;

class Component {
private:
	GameObject * m_parent;

public:
	Component(GameObject * parent = nullptr) {
		this->m_parent = parent;
	}

	virtual ~Component() {
		m_parent = nullptr;
	}

	GameObject* GetParent() {
		return m_parent;
	}

	void SetParent(GameObject* parent) {
		this->m_parent = parent;
	}

	virtual std::string getInfo() {
		return "Component";
	}
};

class SoundSource : public Component {
private:
	// some sound specific members

public:
	SoundSource(GameObject * parent = nullptr) : Component(parent) {
	}

	virtual ~SoundSource() {
	}

	std::string getInfo() override {
		return "SoundSource";
	}
};

class MeshLoader : public Component {
private:

	std::string* m_filepath;
	void Load() {

	}

public:
	MeshLoader(std::string filepath) {
		m_filepath = &filepath;
	}

	MeshLoader() {

	}

	~MeshLoader() {

	}

	std::string getInfo() override {
		return "MeshLoader";
	}

	void setFile(std::string filepath) {
		m_filepath = &filepath;
	}

	std::string getFile() {
		return *m_filepath;
	}

};

class GameObject {
private:
	std::vector<Component*> m_Components;

public:
	GameObject() :
		m_Components() {
	}

	~GameObject() {
		for (size_t i = 0; i < m_Components.size(); ++i)
			if (m_Components[i]->GetParent() == this) delete m_Components[i];
		m_Components.clear();
	}


	void AddComponent(Component* comp) {
		if (comp->GetParent() == nullptr) {
			comp->SetParent(this);
		}
		m_Components.push_back(comp);
	}

	template <class component_type>
	component_type* GetComponent() {
		for (auto comp : m_Components) {
			component_type* c = dynamic_cast<component_type*>(comp);
			if (c) return c;
		}
		return nullptr;
	}
};