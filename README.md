# GDC2016

Plug-in that implements the communication aspects of the Epic Games McLaren demo
at GDC 2016.

[![McLaren 570S Unreal Engine 4 Cinematic Trailer](http://img.youtube.com/vi/l4FiJ1A5veY/0.jpg)](https://www.youtube.com/watch?v=l4FiJ1A5veY)

(click image to watch the [cinematic trailer](https://www.youtube.com/watch?v=l4FiJ1A5veY))


## About

This plug-in contains the code that was used to implement the communication
between the McLaren configurator on PC and its companion app on iPad. We provide
it only for inspiration and as a learning resource for those who are curious to
know how the feature was implemented. This plug-in won't be useful for anything
else, but you can use it as a basis for your own projects and experiments.

For a brief introduction about the making of this project, please check out our
[Twitch livestream](https://www.youtube.com/watch?v=tjvKsEcbHk0) on YouTube. An
overview of the programming details starts at [43:45](https://youtu.be/tjvKsEcbHk0?t=2625).

Make sure to pull the Tag that matches your Unreal Engine version. If you sync
to Master the code may not compile, because it may depend on Engine changes that
are not yet available in the UE4 Master branch.


## Supported Platforms

This plug-in was last built against **Unreal Engine 4.17** and works on all
platforms.


## Overview

The configurator and the companion app were implemented using different versions
of Unreal Engine 4, which prevented us from using simple network replication to
have the two communicate with each other. As a workaround, we decided to use the
*Unreal Message Bus* instead, a system in Unreal Engine 4 that has been around
for a while, but is currently only used in Unreal Editor and Unreal Frontend.
Because it is still somewhat experimental, we've been keeping it under wraps.

As described in the Twitch livestream, the purpose of the message bus is to
provide a mechanism for distributed applications to communicate with each other
while hiding the implementation details of low-level network transports, such as
sockets, IP addresses, and port numbers in order to provide a better experience
to the user. As a programmer, however, even messages are still too low-level.

What we are really interested in is something that works similar to network
replication: the programmer simply calls a C++ function, which may then execute
on another computer. We are currently implementing such a system on top of the
message bus: [MessagingRpc](https://github.com/EpicGames/UnrealEngine/tree/master/Engine/Source/Runtime/MessagingRpc).
Although it is complete overkill for something as simple as the McLaren demo, we
decided to use it anyway as an early test bed. Most communication therefore
happens not via messages directly, but via RPC calls that send messages under
the hood.


## Implementation Details

The plug-in contains two classes, *AConfiguratorGameMode* and
*ACompanionGameMode*, that expose a number of Blueprint functions in the
configurator and companion app respectively. For convenience, we put both into
a single plug-in, which is compiled seperately for each project. The game
designer then selects the appropriate game mode in each project. Because the
message bus implementation slightly differs between the two, we had to add a
[small hack](https://github.com/ue4plugins/GDC2016/blob/master/Source/GDC2016/Private/ConfiguratorGameMode.cpp#L30)
to make sure the code compiles in both projects.

For the purpose of the demo, the communication was kept unidirectional, meaning
that the companion app only sends commands, and the configurator only receives
and processes them. Return values from RPC calls are ignored. Other than
discovering the configurator there is no communication from the configurator
back to the companion app. However, if you need bi-directional communication,
you can easily achieve it by following the same principles.

The connection from the companion app to the configurator happens automatically
in [ACompanionGameMode::HandleTicker](https://github.com/ue4plugins/GDC2016/blob/master/Source/GDC2016/Private/CompanionGameMode.cpp#L88),
which continuously pings the message bus for available configurators. To keep
things simple, this discovery mechanism does not support multiple configurators
on the network. It assumes that there is only one instance at a time and
connects to the first one that it finds.

MessagingRpc is still in an early stage and missing some important features. In
particular, it does not currently have a code generator in UHT that generates
the messages from C++ declarations of RPC functions. We therefore had to code
[all the messages](https://github.com/ue4plugins/GDC2016/blob/master/Source/GDC2016/Private/RpcMessages.h)
by hand. In order to get the project up and running quickly, we also made a big
simplification in that there is only a single command that contains all possible
user interactions (*FConfiguratorCommandRequest*) distinguished by an enum value
(*EConfiguratorCommand*).

In the future, there will be syntax to declare RPC functions, and messages (one
per function) will be generated automatically.


## Support

No support is provided for this plug-in, and we're not planning to add any new
features. If you have questions about the McLaren project, please visit the
[UE4 Forums](https://forums.unrealengine.com/showthread.php?109912-Max-Preussner-s-powerpoint-from-The-Making-of-the-McLaren-Car-Configurator), thanks!

## References

* [Slides from the Twitch Livestream](http://www.slideshare.net/GerkeMaxPreussner/ue4-twitch-2016-0505-unreal-message-bus-overview) (SlideShare)
* [GDC 2016: A Real-Time Rendered Future - McLaren + Unreal Engine 4](https://www.youtube.com/watch?v=0oZfvoG6qYQ) (YouTube)
* [IMessageBus Interface](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/IMessageBus/index.html) (UE4 Docs)
