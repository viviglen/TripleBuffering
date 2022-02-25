# TripleBuffering
[Triple Buffering as a Concurrency Mechanism] test code

test code for http://remis-thoughts.blogspot.com/2012/01/triple-buffering-as-concurrency.html and patch with http://remis-thoughts.blogspot.com/2012/01/triple-buffering-as-concurrency_30.html

“Triple Buffering is a way of passing data between a producer and a consumer running at different rates. It ensures that the consumer only sees complete data with minimal lag, but the consumer doesn't expect to see every piece of data. The technique is most commonly used between images produced by a graphics card and a monitor; the graphics card is free to render hundreds of frames per second while the monitor consumes a fixed 60 fps.

This technique is also applicable as a small-scale lock-free concurrency mechanism; many applications consume real-time data but want to operate on fixed snapshots, or alternatively the data-processing operation performed takes longer than the time between each new piece of input data (and missing input data is acceptable). ”

============== result ==============
```shell
64,3
74,3
75,3
76,3
78,6
79,6
86,6
87,6
88,6
90,8
92,8
```
