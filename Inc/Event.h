#ifndef __EVENT_H
#define __EVENT_H

typedef enum EventType EventType;
enum EventType {
  EVT_TIMEOUT,
  EVT_RECEIVE,
};

typedef struct Event Event;
struct Event {
  EventType type;
  void *data;
};

#endif /* __EVENT_H */
