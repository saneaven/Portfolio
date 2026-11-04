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
let nowmap = 0;
let status = 6;
let blocks = [];
let block_sample = new _block(0,0);
let ball = new _ball(100,48);
let refreshMap = true;
let spawnPoint = new _vec2(0,0);
let isDelaying = false;
let isStart = true;
let health = 10;
let score = 0;
let fps = 60;

function preload(){
  basic_block = loadImage('images/basic_block.png');
  bomb_box = loadImage('images/bomb_box.png');
  bouncy_block = loadImage('images/bouncy_block.png');
  left_arrow_block = loadImage('images/left_arrow_block.png');
  right_arrow_block = loadImage('images/right_arrow_block.png');
  stone_block = loadImage('images/stone_block.png');
  intro_fixed_image = loadImage('images/intro_fixed_image.png');
  intro_moving_image = loadImage('images/intro_moving_image.png');
  stage_image = loadImage('images/stage.PNG');
  zero = loadImage('images/zero.PNG');
  one = loadImage('images/one.PNG');
  two = loadImage('images/two.PNG');
  three = loadImage('images/three.PNG');
  four = loadImage('images/four.PNG');
  five = loadImage('images/five.PNG');
  six = loadImage('images/six.PNG');
  seven = loadImage('images/seven.PNG');
  eight = loadImage('images/eight.PNG');
  nine = loadImage('images/nine.PNG');
  game_over_image = loadImage('images/game_over.PNG');
  map1_image = loadImage('images/map1.png');
  map2_image = loadImage('images/map2.png');
  map3_image = loadImage('images/map3.png');
  map4_image = loadImage('images/map4.png');
  map5_image = loadImage('images/map5.png');
  map6_image = loadImage('images/map6.png');
  map7_image = loadImage('images/map7.png');
  map8_image = loadImage('images/map8.png');
  map9_image = loadImage('images/map9.png');
  map10_image = loadImage('images/map10.png');
  
  arrow_block_sound = loadSound('sounds/arrow_block.mp3');
  ball_bounce_sound = loadSound('sounds/ball_bounce.mp3');
  ball_dead_sound = loadSound('sounds/ball_dead.mp3');
  ball_wall_sound = loadSound('sounds/ball_wall.mp3');
  bomb_box_sound = loadSound('sounds/bomb_box.mp3');
  bouncy_block_sound = loadSound('sounds/bouncy_block.mp3');
  brake_sound = loadSound('sounds/brake_sound.mp3');
  break_block_sound = loadSound('sounds/break_block.mp3');
  clear_stage_sound = loadSound('sounds/clear_stage.mp3');
  intro_sound = loadSound('sounds/intro_sound.mp3');
  next_stage_sound = loadSound('sounds/next_stage.mp3');
  wall_jump_sound = loadSound('sounds/wall_jump.mp3');
}

function setup() 
{
  // put setup code here
  createCanvas(640,480);
  InitMap(0);
  rectMode(CENTER);
  imageMode(CENTER);
}

function draw()
{
  // put drawing code here
  switch(status)
  {
    case 0://ingame
      InGame(ball, blocks);
      break;

    case 1://reload the game
      refreshMap = true;
      InitMap(nowmap);
      ball = new _ball(spawnPoint.x,spawnPoint.y);
      MapLoadAnimation();
      break;

    case 2://goto next map
      nowmap++;
      if(nowmap > 9)
        status = 9;
      else
        status = 7;
      break;

    case 3://take 1 health
      score += 30;
      health--;
      status = 8;
      if(health < 0)
      {
        status = 4;
      }
      break;

    case 4://Game over
      background("black");
      image(game_over_image, width/2, height/2, 100, 50);
      if(keyIsDown(LEFT_ARROW))
      {
        nowmap = 0;
        status = 6;
        health = 10;
        score = 0;
      }
      else if(keyIsDown(RIGHT_ARROW))
      {
        window.location.reload();
      }
      break;
      
    case 5://explosion
      Explosion();
      break;
    case 6://loading(no animation)
    case 7://loading(animation from leftside)
    case 8://loading(animation from uppper side)
      StageScreen(status - 6);  
      break;
    case 9://Ending
      GameEnding();
  }
}