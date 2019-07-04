Ironbook (WIP)
---

Ironbook is a price-time limit order book implemented in C11.

# Description #

Ironbook is both a library and an executable. The library allows programmatic
access to the matching engine and its associated data structures. The executable
is a program which accepts orders via the network.

## Matching Engine ##

Orders are stored across two separate doubly-linked lists, one for bids and one
for asks. Bids are sorted in ascending order by price level and asks are sorted
in descending order. Price levels are maintained separately in their own lists.

Each element of these two lists is a queue. This provides time priority by
default. Finally, each element of these queues are orders themselves.

The matching engine accepts an order and attempts to match it against the
opposite side of the order book. Inability to match is indicated by its own
status code. Ironbook supports partial matching, i.e. an incoming order may be
matched against several smaller orders on the opposing side of the book and may
remain open if there is insufficient counterparty volume.

