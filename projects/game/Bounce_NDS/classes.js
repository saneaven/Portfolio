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

class _vec2 //vector class
{
  constructor(x,y)
  {
    this.x = x || 0;
    this.y = y || 0;
  }
  
  //It MUST be GetAngle but...
  getAngle()
  {
    return atan2(this.y,this.x);
  }
  
  setAngle(angle_in_radians)
  {
    // let distance = sqrt(this.x*this.x + this.y*this.y)
    let distance = this.getLength();
    
    this.x = distance * cos(angle_in_radians);
    this.y = distance * sin(angle_in_radians);
  }
  
  getLength()
  {
    return sqrt(this.x*this.x + this.y*this.y);
  }
  setLength(length)
  {
    let angle = this.getAngle();
    
    this.x = length * cos(angle);
    this.y = length * sin(angle); 
  }
  
  add(v2)
  {
    let v3 = new _vec2();
    
    v3.x = this.x + v2.x;
    v3.y = this.y + v2.y;    
    return v3;
  }
  
  addTo(v2)
  {
    this.x += v2.x;
    this.y += v2.y;
    
    return this;
  }
  
  subtract(v2)
  {
    let v3 = new _vec2();
    
    v3.x = this.x - v2.x;
    v3.y = this.y - v2.y;    
    
    return v3;
  }
  
  subtractFrom(v2)
  {
    this.x -= v2.x;
    this.y -= v2.y;
    
    return this;
  }
  
  multiply(scalar)
  {    
    let v3 = new _vec2();
    
    v3.x = this.x*scalar;
    v3.y = this.y*scalar;    
    return v3;
  }
  
  multiplyBy(scalar)
  {
    this.x *= scalar;
    this.y *= scalar;
    
    return this;
  }
  
  divide(scalar)
  {
    let v3 = new _vec2();
    
    v3.x = this.x/scalar;
    v3.y = this.y/scalar;    
    return v3;
  }
  
  divideBy(scalar)
  {
    this.x /= scalar;
    this.y /= scalar;
    
    return this;
  }
  
  innerProduct(v2)
  {
    return this.x*v2.x + this.y*v2.y;   
  }
 
  draw(mx,my)
  {
    if(this.getLength() != 0)
    {
    let x = mx || 0;
    let y = my || 0;
    push();
    
    translate(x,y);
    rotate(this.getAngle());
    
    line(0,0, this.getLength(),0);
    line(this.getLength(),0, this.getLength()-10,10);
    line(this.getLength(),0, this.getLength()-10,-10); 
    
    pop();
    }
  } 
}

class _ball //ball(main charactor)
{
    constructor(x,y,type)
    {
        this.pos = new _vec2(x,y);
        this.v = new _vec2(0,0);
        this.a = new _vec2(0,0);
        this.g = new _vec2(0,10/60);
        this.type = type || 0;
        this.rad = 4;
        this.const_accel = 7/60;
        this._MX_x = 2.2;
        this._MX_y = 3.6;

        this.maxSpeedX = this._MX_x;
        this.maxSpeedY = this._MX_y;

        this.ablewallJump = false;
        this.isGravity = true;
        this.tale = [];
        this.track = [];
    }

    move()
    {
      this.pos.addTo(this.v);
    }
    
    move_vec(dv)
    {
      this.pos.addTo(dv);
    }
    

    setVelocity(vec)
    {
      this.x = vec.x;
      this.y = vec.y;
    }
    
    accel()
    {
        this.v.addTo( this.a );
        this.checkMaxSpeed();
    }
    
    gravity()
    {
      if(this.isGravity)
      {
        this.v.addTo( this.g );
        this.checkMaxSpeed();
      }
    }

    checkMaxSpeed()
    {
        if(this.v.x > this.maxSpeedX)
        {
            this.v.x = this.maxSpeedX;
        }
        else if(this.v.x < -this.maxSpeedX)
        {
            this.v.x = -this.maxSpeedX;
        }
        if(this.v.y > this.maxSpeedY)
        {
            this.v.y = this.maxSpeedY;
        }
        else if(this.v.y < -this.maxSpeedY)
        {
            this.v.y = -this.maxSpeedY;
        }
    }

    defineAccel(x,y)
    {
        this.a = _vec2(x,y);
    }

    getInput()
    {

        this.a.x = 0;

        if(keyIsDown(LEFT_ARROW) && keyIsDown(RIGHT_ARROW))
        {  
          this.ableGravity(); 
          this.a.x = 0;
        }
        else if(keyIsDown(LEFT_ARROW))
        {  
          if(!this.isGravity)
          {
            brake_sound.play();
            this.ableGravity(); 
          }
          if(ball.v.x < 0 && this.ableWallJump)
          {
            this.wallJump();
            this.ableWallJump = false;
          }
          this.a.x = -this.const_accel;
        }
        else if(keyIsDown(RIGHT_ARROW))
        {   
          if(!this.isGravity)
          {
            brake_sound.play();
            this.ableGravity(); 
          }
          if(ball.v.x > 0 && this.ableWallJump)
          {
            this.wallJump();
            this.ableWallJump = false;
          }
          this.a.x = this.const_accel;
        }
    }

    ableBouncy()
    {
      this.maxSpeedY = this._MX_y * 1.7;
    }

    disableBouncy()
    {
      this.maxSpeedY = this._MX_y;
    }

    ableGravity()
    {
      this.maxSpeedX = this._MX_x;
      this.isGravity = true;
    }

    disableGravity()//using in arrow block
    {
      this.maxSpeedX = this._MX_x * 2;
      this.isGravity = false;
    }

    wallJump()
    {
      let i = 0;

      this.v.x = -(this.v.x/abs(this.v.x)) * this.maxSpeedX * 3/2; // reverse direction and set to max speed x
      this.v.y = -this.maxSpeedY / 3;

      while(i < 10 && !Collision(ball, blocks))
      {
        this.move();
        this.draw();
        i++;
      }
      this.v.x = (this.v.x/abs(this.v.x)) * this.maxSpeedX;
      this.v.y = -this.maxSpeedY * 3/4;
    }
    
    drawTale()
    {
      for(let i = 0; i < this.tale.length; i++)
      {
        push();
        fill(0x11);
        // circle(this.tale[i].x,this.tale[i].y,this.rad*2);
        quad( this.tale[i].x - this.rad/2, this.tale[i].y - this.rad/2, 
            this.tale[i].x + this.rad/2, this.tale[i].y + this.rad/4, 
            this.tale[i].x + this.rad/2, this.tale[i].y + this.rad/2, 
            this.tale[i].x - this.rad/2, this.tale[i].y - this.rad/4);
        pop();
      }

      if(this.tale.length > 1)//to make tale look denser
      {
        this.tale.push( new _vec2( (this.tale[this.tale.length - 1].x + this.pos.x)/2, (this.tale[this.tale.length - 1].y + this.pos.y)/2 ) );
        this.track.push( new _vec2( (this.track[this.track.length - 1].x + this.pos.x)/2, (this.track[this.track.length - 1].y + this.pos.y)/2 ) );
      }


      this.tale.push(new _vec2(this.pos.x,this.pos.y));
      this.track.push(new _vec2(this.pos.x,this.pos.y));

      if(this.tale.length > 50)
        this.tale.splice(0,2);
      
      for(let i = 0; i < this.tale.length; i++)
      {
        push();
        fill(0xff - (0xee/50) * (this.tale.length - i));
        // circle(this.tale[i].x,this.tale[i].y,this.rad*2);
        quad( this.tale[i].x - this.rad/2, this.tale[i].y - this.rad/2, 
            this.tale[i].x + this.rad/2, this.tale[i].y + this.rad/4, 
            this.tale[i].x + this.rad/2, this.tale[i].y + this.rad/2, 
            this.tale[i].x - this.rad/2, this.tale[i].y - this.rad/4);
        pop();
      }
    }

    redrawTrack()//when break the stone block, screen refreshes. and make track draw again
    {
      for(let i = 0; i < this.track.length; i++)
      {
        push();
        noStroke();
        fill(0x11);
        quad( this.track[i].x - this.rad/2, this.track[i].y - this.rad/2, 
              this.track[i].x + this.rad/2, this.track[i].y + this.rad/4, 
              this.track[i].x + this.rad/2, this.track[i].y + this.rad/2, 
              this.track[i].x - this.rad/2, this.track[i].y - this.rad/4);
        pop();
      }
    }
    draw()
    {
      push();
      noStroke();
      this.drawTale();
      fill("#ffffff");
      // circle(this.pos.x,this.pos.y,this.rad*2);
      quad( this.pos.x - this.rad/2, this.pos.y - this.rad/2, 
            this.pos.x + this.rad/2, this.pos.y + this.rad/4, 
            this.pos.x + this.rad/2, this.pos.y + this.rad/2, 
            this.pos.x - this.rad/2, this.pos.y - this.rad/4);
      pop();
    }
}

class _block // type : 0 = basic, 1 = stone, 2 = bouncy, 3 = left, 4 = right, 5 = bomb
{
    constructor(x,y,type)
    {
        this.x = x;
        this.y = y;
        this.length = 32;
        this.type = type || 0;
    }
    


    draw()
    {
      imageMode(CENTER);
      push();
      switch(this.type)
      {
      case 0:
        image(basic_block, this.x, this.y, this.length, this.length);
        break;
      case 1:
        image(stone_block, this.x, this.y, this.length, this.length);
        break;
      case 2:
        image(bouncy_block, this.x, this.y, this.length, this.length);
        break;
      case 3:
        image(left_arrow_block, this.x, this.y, this.length, this.length);
        break;
      case 4:
        image(right_arrow_block, this.x, this.y, this.length, this.length);
        break;
      case 5:
        image(bomb_box, this.x, this.y, this.length, this.length);
      }
      pop();
    }
}