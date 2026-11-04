// gam100_minjae.kyung_game
// BOUND
// GAM100
// Fall 2019
// Primary author : minjae.kyung
// Secondary author : junhyuk.cha
// “All content © 2019 DigiPen (USA) Corporation, all rights reserved.”


/* jslint esversion: 6 */
/// <reference path="p5.js" />
/// <reference path="p5.dom.js" />
/// <reference path="classes.js" />

function IsBallHitsEdge(ball, block)
{
    let dx_temp = ball.pos.x - block.x;
    let dy_temp = ball.pos.y - block.y;

    for(i = 0; i<4;++i)//check if every blocks edge in ball 
    {
        switch(i)
        {
        case 0:
            dx = dx_temp + block.length/2;
            dy = dy_temp + block.length/2;
            break;
        case 1:
            dx = dx_temp + block.length/2;
            dy = dy_temp - block.length/2;
            break;
        case 2:
            dx = dx_temp - block.length/2;
            dy = dy_temp + block.length/2;
            break;
        case 3:
            dx = dx_temp - block.length/2;
            dy = dy_temp - block.length/2;
            break;
        }

        let length = sqrt(dx*dx + dy*dy);

        if(length <= ball.rad)
            return true;
    }
    return false;
}   

function Interact(ball, blocks, i, direction)
{
    switch(direction)
    {
    case "x":
        ball_wall_sound.play();
        if(ball.pos.x > blocks[i].x)
        {
            ball.pos.x = blocks[i].x  + blocks[i].length/2 + ball.rad + 1;//move right outside of block to avoid double collision
        }
        else
        {
            ball.pos.x = blocks[i].x - blocks[i].length/2 - ball.rad  - 1;//move left outside of block
        }

        if(blocks[i].type == 5)//bomb block
        {
            status = 2;
        }
        else
        {
            if( (keyIsDown(LEFT_ARROW) && ball.v.x > 0) ||
                (keyIsDown(RIGHT_ARROW) && ball.v.x < 0) ) //pressing left/right arrow both can active walljump
            {
                //print("walljump");
                ball.wallJump();
                wall_jump_sound.play();
                break;
            }
            setTimeout(function()//classic wall jump
            {
                ball.ableWallJump = false;
            }, 100);
            ball.ableWallJump = true;
            ball.v.x *= -1;
        }
        break;

    case "y":
        if(ball.pos.y > blocks[i].y)//if ball hits downside of block
        {
            ball.pos.y = blocks[i].y  + blocks[i].length/2 + ball.rad + 1;//move ball outside

            if(blocks[i].type == 5)//bomb
            {
                status = 2;
            }
            else
            {
                ball.v.y = abs(ball.v.y);
            }

        }
        else//hits upside
        {
            score++;
            ball.pos.y = blocks[i].y - blocks[i].length/2 - ball.rad  - 1;//move ball outside

            switch(blocks[i].type)//react differently by block's type
            {
            case 0://normal
                ball.v.y = -ball.maxSpeedY;
                ball_bounce_sound.play();
                break;
            case 1://stone
                ball.v.y = -ball.maxSpeedY;
                blocks.splice(i,1);
                break_block_sound.play();
                refreshMap = true;
                break;
            case 2://bouncy
                ball.ableBouncy();
                ball.v.y = -ball.maxSpeedY;
                bouncy_block_sound.play();
                break;
            case 3://left arrow
                ball.disableGravity();
                ball.pos.x = blocks[i].x  - blocks[i].length/2 - ball.rad - 1;
                ball.pos.y = blocks[i].y;
                ball.v.x = -ball.maxSpeedX;
                ball.v.y = 0;
                arrow_block_sound.play();
                break;
            case 4://right arrow
                ball.disableGravity();
                ball.pos.x = blocks[i].x  + blocks[i].length/2 + ball.rad + 1;
                ball.pos.y = blocks[i].y;
                ball.v.x = ball.maxSpeedX;
                ball.v.y = 0;
                arrow_block_sound.play();
                break;
            case 5://bomb
                status = 5;
                bomb_box_sound.play();

            }
        }
        break;

    case "edge"://edge hits equals to up/downside hit
        Interact(ball, blocks, i, "y");
    }
}

function Collision(ball, blocks)
{
    let IsHittedOnlyEdge = false;
    let hitBlock = 0;
    let IsHitted = false;
    
    for(let i = 0; i < blocks.length; ++i)
    {
        if(  blocks[i].x - blocks[i].length/2 < ball.pos.x + ball.rad && ball.pos.x - ball.rad < blocks[i].x + blocks[i].length/2  &&
             blocks[i].y - blocks[i].length/2 < ball.pos.y + ball.rad && ball.pos.y - ball.rad < blocks[i].y + blocks[i].length/2    )
        {//check ball is in big square
            ball.disableBouncy();
            ball.ableGravity();

            if( blocks[i].y - blocks[i].length/2 < ball.pos.y && ball.pos.y < blocks[i].y + blocks[i].length/2 )
            {//check ball hits the right/leftside
                IsHittedOnlyEdge = false;
                Interact(ball, blocks, i, "x");
            }
            else if( blocks[i].x - blocks[i].length/2 < ball.pos.x && ball.pos.x < blocks[i].x + blocks[i].length/2 )
            {//check ball hits the up/downside
                IsHittedOnlyEdge = false;
                Interact(ball, blocks, i, "y");
            } 
            else if( IsBallHitsEdge(ball, blocks[i]) )
            {//check ball only hits only edge(protect double collision with attached block)
                IsHittedOnlyEdge = true;
                hitBlock = i;
            }

            IsHitted = true;
        }
    }

    if(IsHittedOnlyEdge)
        Interact(ball, blocks, hitBlock, "edge");

    return IsHitted;
}

