in this example we solve the problem where the barrier must be ready to be used again, for example, within a cycle. You must ensure that the counter is left at 0, and threads leaving the barrier are not merged with others that are still in the barrier.

In order to do that we can replace the barrier with two turnstiles. A turnstile or "top" is an access control device that prevents or gives way to a sequence of people according to certain conditions. It is typically used in sports facilities, public transportation, building access, amusement parks, and others.

A turnstile is a semaphore that stops all threads reaching it. But as soon as one thread gets past it, it immediately enables the next thread to pass. It can be thought of as a more elegant way for the last thread to reach the barrier to cycle increments through all the others.