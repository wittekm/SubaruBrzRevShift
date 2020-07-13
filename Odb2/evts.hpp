#pragma once

struct Event {
    int type;
    char payload;
    Event(int type, char payload): type(type), payload(payload) { }

    bool isEmpty() {
        return type == 0;
    }

    static Event getDefault() {
        return Event(0, 0);
    }
};

enum EventType {
    KB = 1
};

class EventQueue {
private:
    xQueueHandle queue = NULL;

public:
    EventQueue()
    {
        queue = xQueueCreate(10, sizeof(Event));
    }

    void add(Event event) {
       int result = xQueueSend(queue, &event, 0);
    }

    Event get() {
        Event event = Event::getDefault();
        bool result = xQueueReceive(queue, &event, 0);
        if (result == pdFALSE) {
            return Event::getDefault();
        }
        return event;
    }
};