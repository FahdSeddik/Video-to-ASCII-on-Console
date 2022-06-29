# Video to ASCII Console Animator

This C++ program converts a .mp4 file to text-art files using [Image-To-TextArt](https://github.com/FahdSeddik/Image-To-TextArt) tool that I previously developed.
I recommend checking details about that project first.



 https://user-images.githubusercontent.com/62207434/176327257-d8e3fee2-1b84-484a-8946-b921a385b887.mp4



## Process

The video is broken up into frames and each are processed like a normal image would be processed in the [Image-To-TextArt](https://github.com/FahdSeddik/Image-To-TextArt)
project.

- DisplayFiles directory contains:
    1. Text files numbered according to frame number
    2. size.txt has console size setting used to render previous video

## Instructions

- Videos to be rendered should be placed in the same directory as "Main.cpp"  
- At program start you would be asked to enter the video name please include ".mp4" at the end of the file name  
- You should clear and process the video from the beginning in 2 cases:  
    1. First time rendering the video
    2. You want to change render text size 

- **2 Videos are already provided that you can render**:  
    - Noot.mp4  
    - Paint.mp4
    
 **Note: small text size might cause screen stuttering due to printing speed of console**  
 ## Tasks
 
 - [x] <s>Added sleep delay text size 6</s>
 - [ ] Implement efficient console printing

