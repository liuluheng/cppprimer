#include <set>

class Message;

class Folder {
public:
    void addMsg(Message*);
    void remMsg(Message*);

private:
    std::set<Message*> messages;
}

void Folder::addMsg(Message *m)
{
    messages.insert(m);
}

void Folder::remMsg(Message *m)
{
    messages.erase(m);
}
