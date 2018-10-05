#include "MsgAndFolder.h"

Message::Message(const Message & msg)
	:contents(msg.contents),folders(msg.folders)
{
	add_to_Folders(msg);
}

Message & Message::operator=(const Message & msg)
{
	remove_from_Folders();
	contents = msg.contents;
	folders = msg.folders;
	add_to_Folders(msg);
	return *this;
}

Message::~Message()
{
	remove_from_Folders();
}

void Message::save(Folder & f)
{
	folders.insert(&f);
	f.addMsg(this);
}

void Message::remove(Folder & f)
{
	folders.erase(&f);
	f.remMsg(this);
}

void Message::addFolder(Folder * f)
{
	f->addMsg(this);
}

void Message::remFolder(Folder * f)
{
	f->remMsg(this);
}

void Message::swap(Message & lhs, Message & rhs)
{
	using std::swap;
	for (auto f : lhs.folders)
		f->remMsg(&lhs);
	for (auto f : rhs.folders)
		f->remMsg(&rhs);
	swap(lhs.folders, rhs.folders);
	swap(lhs.contents, rhs.contents);

	for (auto f : lhs.folders)
		f->addMsg(&lhs);
	for (auto f : rhs.folders)
		f->addMsg(&rhs);
}

void Message::add_to_Folders(const Message & msg)
{
	for (auto f : msg.folders)
		f->addMsg(this);
}

void Message::remove_from_Folders()
{
	for (auto f : folders)
		f->remMsg(this);
}

Folder::Folder(const Folder & f):messages(f.messages)
{
	add_to_Msg();
}

Folder & Folder::operator=(const Folder & rhs)
{
	remove_from_Msg();
	messages = rhs.messages;
	add_to_Msg();
	return *this;
}

Folder::~Folder()
{
	remove_from_Msg();
}

void Folder::addMsg(Message * Msg)
{
	messages.insert(Msg);
}

void Folder::remMsg(Message * Msg)
{
	messages.erase(Msg);
}

void Folder::add_to_Msg()
{
	for (auto msg : messages)
		msg->folders.insert(this);
}

void Folder::remove_from_Msg()
{
	for (auto msg : messages)
		msg->folders.erase(this);
}

std::ostream & operator<<(std::ostream &os, const Folder & f)
{
	for (auto msg : f.messages)
		os << msg->getMsg() << std::endl;
	return os;
}
