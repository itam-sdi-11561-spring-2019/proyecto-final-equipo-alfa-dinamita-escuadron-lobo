#include <ros.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Int32.h>

#define E1 10     // Enable Pin for motor 1
#define I1 8     // Control pin 1 for motor 1
#define I2 9     // Control pin 2 for motor 1
#define E2 11
#define J1 12
#define J2 13

#define max_vel 33.5103216383

#define pi 3.14159265359

float v_der = 0, v_izq = 0, integral_der = 0, derivada_der = 0, integral_izq=0,derivada_izq=0;
float Kp_der = 20, Kp_izq = 20, Kd_der=10, Kd_izq=10,Ki_der=30,Ki_izq=30;

float entrada_derecho, entrada_izquierdo;

int fotodiodod = A0;
int cuentad = 0;
int anteriord = 0;
int actuald = 0;

int fotodiodoi = A1;
int cuentai = 0;
int anteriori = 0;
int actuali = 0;

int m1_adelante = HIGH;
int m1_atras = LOW;
int m2_adelante = HIGH;
int m2_atras = LOW;
int vel1 = 0;
int vel2 = 0;

unsigned long i;
unsigned long f;

void callback(geometry_msgs::Pose2D &pose)
{
  if(pose.x < 0)
  {
    m2_adelante = LOW;
    m2_atras = HIGH;
  }
  else
  {
    m2_adelante = HIGH;
    m2_atras = LOW;
  }
  if(pose.y < 0)
  {
    m1_adelante = LOW;
    m1_atras = HIGH;
  }
  else
  {
    m1_adelante = HIGH;
    m1_atras = LOW;
  }
  vel2 = abs(pose.x)/max_vel*255;
  vel1 = abs(pose.y)/max_vel*255;
}

std_msgs::Int32 ok_msg;

ros::NodeHandle nh;
ros::Subscriber<geometry_msgs::Pose2D> sub("/pose_dif", &callback);
ros::Publisher pub("/ok_msg", &ok_msg);

void setup()
  {
      nh.initNode();
      nh.subscribe(sub);
      nh.advertise(pub);
      i = millis();
      entrada_izquierdo = 0;
      entrada_derecho = 0;
      pinMode(fotodiodod, INPUT);
      pinMode(fotodiodoi, INPUT);
      for (int i = 8 ; i<14 ; i++)
        pinMode( i, OUTPUT);
  }

void loop()
  {   
      // NOTA: la secuencia HIGH-LOW genera movimiento hacia adelante y la secuencia LOW-HIGH genera movimiento hacia atrás.
      // Escribir los valores de velocidad
      if(entrada_derecho == 0 && entrada_izquierdo == 0)
      {
        entrada_derecho = vel1;
        entrada_izquierdo = vel2;
      }
      
      analogWrite(E1, entrada_derecho);
      digitalWrite(I1, m1_adelante);
      digitalWrite(I2, m1_atras);
      analogWrite(E2, entrada_izquierdo);
      digitalWrite(J1,m2_adelante);
      digitalWrite(J2, m2_atras);

      f = millis();
      int milisegundos = 20;
      if(f-i >= milisegundos)
      {
        i = millis();
        v_izq = cuentai/36*2*pi*1000/milisegundos;
        cuentai = 0;
        integral_izq += vel2-v_izq;
        entrada_izquierdo = (vel2-v_izq)*Kp_izq + Ki_izq*integral_izq + Kd_izq*(vel2-v_izq-derivada_izq);
        derivada_izq = vel2-v_izq;
        
        v_der = cuentad/36*2*pi*1000/milisegundos;
        cuentad = 0;
        integral_der += vel1-v_der;
        entrada_derecho = (vel1-v_der)*Kp_der+Ki_der*integral_der+Kd_der*(vel1-v_der-derivada_der);
        derivada_der = vel1-v_der;
      }
      
      // Leer el estado del fotodiodo
      actuald = digitalRead(fotodiodod);
      if(anteriord != actuald)
        cuentad += 1;
      anteriord = actuald;
      actuali = digitalRead(fotodiodoi);
      if(anteriori != actuali)
        cuentai += 1;
      anteriori = actuali;
      
      ok_msg.data = cuentad;
      pub.publish(&ok_msg);
      nh.spinOnce();
  }

