#ifndef __IMESSAGEDISPATCHER_H_
#define __IMESSAGEDISPATCHER_H_

/**
 *@brief A class to manage incoming messages by either handling them of  dispatching them to queues,
 *sending messages at the right place*/
class IMessageDispatcher {

  public:
    virtual ~IMessageDispatcher() = default;

    /**
     *@brief deserialize a message and dispatches it in the appropriate queue
     *@param networkForwarding: flag to forward message undestined message to network.
     *@return true if the serialization and the dispatch was successful, false if any of those were
     *not*/
    virtual bool deserializeAndDispatch(bool networkForwarding) = 0;
};
#endif // __IMESSAGEDISPATCHER_H_
