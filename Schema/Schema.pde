PImage palla;
void setup() {
  size(displayWidth, displayHeight);
  textAlign(CENTER, CENTER);
  imageMode(CENTER);
  palla = loadImage("palla.png");
}

void draw() {
  background(255);
  noFill();
  ellipse(950, height/2-220, 600, 40);
  fill(0, 200, 200);
  rect(20, height/2-80, 200, 80);
  line(220, height/2-40, 420, height/2-40);
  line(420, height/2-40, 600, height/2-40); //sq a

  line(420, height/2-40, 420, height/2-220);
  line(420, height/2-220, 600, height/2-220); //FATO

  line(420, height/2-40, 420, height/2+140);
  line(420, height/2+140, 600, height/2+140); //sq b

  fill(0, 200, 200);
  ellipse(420, height/2-40, 70, 70);
  fill(0, 200, 0);
  rect(600, height/2-260, 200, 80);//Fato
  fill(200, 0, 0);
  rect(600, height/2-80, 200, 80); //Squadra A
  line(800, height/2-40, 900, height/2-40); //Fork della A
  line(900, height/2-40, 900, height/2+10);
  line(900, height/2-40, 900, height/2-90);

  line(900, height/2-40, 950, height/2-40);
  line(900, height/2-65, 950, height/2-65);
  line(900, height/2-90, 950, height/2-90);
  line(900, height/2-15, 950, height/2-15);
  line(900, height/2+10, 950, height/2+10);
  for (int i = 0; i < 5; i++) { //Genero i giocatori
    rect(950, height/2-(25*i), 120, 20);
    line(1070, height/2-(25*i)+10, 1250, height/2+50);
  }
  ellipse(900, height/2-40, 30, 30);


  fill(0, 0, 255);
  rect(600, height/2+100, 200, 80); //Squadra B
  line(800, height/2+140, 900, height/2+140); //Fork della B
  line(900, height/2+140, 900, height/2+190);
  line(900, height/2+140, 900, height/2+90);

  line(900, height/2+140, 950, height/2+140);
  line(900, height/2+165, 950, height/2+165);
  line(900, height/2+190, 950, height/2+190);
  line(900, height/2+115, 950, height/2+115);
  line(900, height/2+90, 950, height/2+90);
  for (int i = 0; i < 5; i++) {  //Genero i giocatori
    rect(950, height/2+(80+25*i), 120, 20);
    line(1070, height/2+(90+25*i), 1250, height/2+50);
  }
  ellipse(900, height/2+140, 30, 30);

  line(1250, height/2+50, 1250, height/4);
  fill(200);
  rect(1140, height/2-260, 200, 80); 
  line(550,height/2-130,700,height/2-180);
  rect(450, height/2- 180, 100, 100);//Log
  image(palla, 1250, height/2+50, 100, 100);
  fill(0);
  textSize(22);
  text("Main\n(Arbitro)", 20, height/2-80, 200, 80);
  //text("Made with Processing 3.2.3", 0, 0, 300, 30); 
  text("Fork", 420, height/2-40);
  text("Squadra A", 600, height/2-80, 200, 80);
  text("Squadra B", 600, height/2+100, 200, 80);
  text("Fato", 600, height/2-260, 200, 80);
  text("log.txt", 450, height/2- 180, 100, 100);
  text("Giocatore A o B", 1140, height/2-260, 200, 80); 
  text("Goal/Dibbling/Infortunio", width/2+width/4-50, height/4);
  text("0/1", width/2+ 300, height/6);
  textSize(14);
  for (int i = 0; i < 5; i++) {
    text("Giocatore A", 950, height/2-(25*i), 120, 20);
    text("Giocatore B", 950, height/2+(80+25*i), 120, 20);
  }
  textSize(8);
  text("Fork", 885, height/2+125, 30, 30);
  text("Fork", 885, height/2-55, 30, 30);
  //saveFrame("schema.png");
}