// gam100_minjae.kyung_game
// BOUND
// GAM100
// Fall 2019
// Primary author : minjae.kyung
// Secondary author : junhyuk.cha
// “All content © 2019 DigiPen (USA) Corporation, all rights reserved.”

/* jslint esversion: 6 */
/// <reference path="p5.js" />
/// <reference path="classes.js" />
/// <reference path="p5.dom.js" />

function InGame(ball, blocks)
{
  
  ball.getInput();

  if(keyIsDown(82))//R to reload game
  {
    status = 1;
    return;
  }
  
  ball.move();
  if(!Collision(ball, blocks))//if collision hit skip accel to avoid bugs
  {
   ball.gravity();
   ball.accel();
  }

  if(abs(ball.v.y) < abs(ball._MX_y))//disable bouncy
  {
    ball.disableBouncy();
  }


  if(refreshMap)//only active when stone broken or reload the map to optimization
  {
    background("#000000");
    for(let i = 0; i < blocks.length; ++i)
    {
      blocks[i].draw();
    }
    ball.redrawTrack();
    refreshMap = false;
  }

  ball.draw();

  if(ball.pos.x > width)//if ball reaches rightside,ball goes next stage
  {
    status = 2;
    return;

  }
  else if(ball.pos.y > height)//if ball fall into downside, ball loose health and reload
  {
    status = 3;
    return;
  }
}

function NumberImage(num)//return each number image(0~9)
{
  let num_image;
  switch(num)
  {
    case 0:
      num_image = zero;
      break;
    case 1:
      num_image = one;
      break;
    case 2:
      num_image = two;
      break;
    case 3:
      num_image = three;
      break;
    case 4:
      num_image = four;
      break;
    case 5:
      num_image = five;
      break;
    case 6:
      num_image = six;
      break;
    case 7:
      num_image = seven;
      break;
    case 8:
      num_image = eight;
      break;
    case 9:
      num_image = nine;
      break;
  }

  return num_image;
}


function PrintNumber(num,x,y)//convert number to image
{
  let n_height = 15;
  let n_width = 10;

  if(num < 10)
  {
    image(NumberImage(num), x, y, n_width, n_height);
  }
  else
  {
    let num_array = [];

    while(num >= 1)
    {
      num_array.push(int(num % 10));
      num /= 10;
    }
    num_array.reverse();
    for(let i = 0; i < num_array.length; i++)
    {
      image(NumberImage(num_array[i]), x - ( (num_array.length/2 - i) * n_width ), y, n_width, n_height);
    }
  }
}

let isLoadingMap = false;
let new_map_moving = 720;
var soundIsPlaying = false;
var image_moving = 0;
var square_moving = 0;
var score_square_moving = 0;
let endAnimation = false;

function StageScreen(direction)//stage screen and animation(direction)
{

  if(isStart)//start loading screen
  {
    background("#000000");
    image(intro_fixed_image, width/2,height/2-100, 100, 50);
    if(!soundIsPlaying)
    {
      intro_sound.play();
      soundIsPlaying = true;
    }
    if(image_moving < 150)
    {
      image_moving = image_moving+2;
    }
    image(intro_moving_image, width/2, height/2+150-image_moving, 100, 100);
    if(square_moving < 50)
    {
      square_moving = square_moving +2;
    }
    push();
    fill('black');
    square(width/2, height/2+150-square_moving, 100);
    pop();
    
  }
  else
  {
    push();
    fill(94, 99, 146);
    noStroke();
 

    switch(direction)//move blue background to drection(0 : nothing, 1: left to right, 2: up to down)
    {
    case 0:
      rect(width/2, height/2, width+20, height);
      endAnimation = true;
      break;
    case 1:

      if(!soundIsPlaying)
      {
        clear_stage_sound.play();
        soundIsPlaying = true;
      }
      if(score_square_moving <= width)
        score_square_moving += 10;

      rect(score_square_moving-width/2, height/2, width+20, height);
      if(score_square_moving >= width)
        endAnimation = true;

      break;
    case 2:
      if(!soundIsPlaying)
      {
        ball_dead_sound.play();
        soundIsPlaying = true;
      }
      if(score_square_moving <= width)
        score_square_moving += 10;

      rect(width/2, score_square_moving-height/2, width+20, height);
      if(score_square_moving >= width)
        endAnimation = true;
      
      break;
    }
    

    imageMode(CENTER);
    if(endAnimation)//stage info
    {
      image(stage_image, width/2-50, height/2-70, 110, 90);
      PrintNumber(nowmap + 1, width/2 + 20, height/2 - 100);
      PrintNumber(health, width/2 + 20, height/2 - 70);
      PrintNumber(score, width/2 + 20, height/2 - 40);
    }
    pop();

  }

  if(direction == 0 && !isStart)
    delay_ms = 1000;
  else
    delay_ms = 3000;

  if(!isDelaying)//animation will ends in 1 or 3sec
  {
    // score_square_moving = 0;
    setTimeout(function()
    {
      soundIsPlaying = false;
      isDelaying = false;
      isLoadingMap = true;
      endAnimation = false;
      if(isStart)
      {
        status = 6;
        isStart = false;
      }
      else
      {
        status = 1;
      }
    }, delay_ms);

    isDelaying = true;
  }
}


function MapLoadAnimation()//map image move from down to up
{
  switch(nowmap)
  {
  case 0:
      image(map1_image, width/2, new_map_moving, width, height);
      break;
  case 1:
      image(map2_image, width/2, new_map_moving, width, height);
      break;
  case 2:
      image(map3_image, width/2, new_map_moving, width, height);
      break;
  case 3:
      image(map4_image, width/2, new_map_moving, width, height);
      break;
  case 4:
      image(map5_image, width/2, new_map_moving, width, height);
      break;
  case 5:
      image(map6_image, width/2, new_map_moving, width, height);
      break;
  case 6:
      image(map7_image, width/2, new_map_moving, width, height);
      break;
  case 7:
      image(map8_image, width/2, new_map_moving, width, height);
      break;
  case 8:
      image(map9_image, width/2, new_map_moving, width, height);
      break;
  case 9:
      image(map10_image, width/2, new_map_moving, width, height);
      break;
  }

  if(!soundIsPlaying)
  {
    next_stage_sound.play();
    soundIsPlaying = true;
  }

  if(new_map_moving > height/2)
    new_map_moving -= 8;
   
  if(!isDelaying)//animation will ends in 1.4sec
  {
    setTimeout(function()
    {
      soundIsPlaying = false;
      score_square_moving = 0;
      status = 0;
      new_map_moving = 720;
      isDelaying = false;
      isLoadingMap = false;
    }, 1400);

    isDelaying = true;
  }  
}

//temp number
let explosion1 = 150;
let explosion2 = 80;
let explosion3 = 10;

function Explosion()//explosion effect and lose health
{
  push();
  noStroke();
  fill(254, 153, 158);
  square(ball.pos.x, ball.pos.y, explosion1);
  fill(255, 184, 142);
  square(ball.pos.x, ball.pos.y, explosion2);
  fill(255, 240, 190);
  square(ball.pos.x, ball.pos.y, explosion3);
  pop();

  if(explosion1 < 288)
  {
    explosion1 += 5;
  }
  if(explosion2 < 216)
  {
    explosion2 += 5;
  }
  if(explosion3 < 144)
  {
    explosion3 += 5;
  }

  if(!isDelaying)
  {
    setTimeout(function()
    {
      explosion1 = 100;
      explosion2 = 50;
      explosion3 = 25;
      soundIsPlaying = false;
      health--;
      score += 30;
      if(health < 0)
        status = 4;
      else
        status = 6;
      isDelaying = false;
    }, 1400);

    isDelaying = true;
  }  
}

function GameEnding()
{
  textAlign(CENTER);
  background("#aaaaff");
  text("You cleared all of our stages!\n congraturations!", width/2, height/2);
}