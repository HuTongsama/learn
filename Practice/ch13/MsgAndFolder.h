#pragma once
#include<string>
#include<set>
#include<iostream>

class Message;
class Folder
{
public:
	Folder() {}
	Folder(const Folder &f);
	Folder& operator=(const Folder &rhs);
	~Folder();
	friend std::ostream& operator<<(std::ostream&, const Folder &f);

	void addMsg(Message* Msg);
	void remMsg(Message* Msg);
private:
	void add_to_Msg();
	void remove_from_Msg();
private:
	std::set<Message*> messages;
};
class Message
{
	friend class Folder;
public:
	explicit Message(const std::string &str = "") :contents(str) {}
	Message(const Message &msg);
	Message& operator=(const Message &msg);
	~Message();
	void save(Folder &f);
	void remove(Folder &f);
	void addFolder(Folder* f);
	void remFolder(Folder* f);
	void swap(Message &lhs, Message &rhs);
	std::string getMsg()const { return contents; }
private:
	void add_to_Folders(const Message &msg);
	void remove_from_Folders();
private:
	std::string contents;
	std::set<Folder*> folders;
};
