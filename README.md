## TDC-pancreas-artificial

UTN FRBA - Teoría de control: Código arduino de simulación de un páncreas artificial basado en la investigación del CONICET

### Demo

- **Demo en wokwi**: [Abrir la demo en wokwi](https://wokwi.com/projects/347496920546017875)
- **Nota**: para la demo se utiliza un potenciometro en lugar del sensor de agua para poder cambiar el valor de medición

#### Imagen de demo con hardware

![image](https://user-images.githubusercontent.com/4097554/200192254-f3120795-0d62-47e2-8c90-13e850537b6e.png)

### Se hace un control PD (proporcional-derivativo)

#### Parámetros

- Valor de referencia: `180 mg/dl`
- Glucosa: medición del sensor de agua mapeado de `0 a 1023` (**sensor agua**) con `50 a 300` (**f**)
- Punto suma: retroalimentación negativa `e = ValorRef - f`
- Proporcional: si `e < 0`
- Derivativo: si `de/dt < -30`

#### Tiempos de medición

- Si los valores de glucemia están entre `120` y `180` (`60` menos del valor de referencia) se toman mediciones cada `500` milisegundos
- Si los valores de glucemia están por debajo de `120` (`60` menos del valor de referencia) se toman mediciones cada `1000` milisegundos
- Si los valores de glucemia supera los `180` (valor de referencia) se esperan `2000` milisegundos para inyección de insulina y `15000` de espera de metabolización

#### Calculo derivativo

- Se calcula en tiempo entre mediciones (`dt = tiempoMedicionActual - tiempoMedcionAnterior`)
- Calculo de `dedt` -> `(e-ePrevio) / (dt / 1000)`
- Si el valor de derivativo (`dedt`) es menor a `-30`

#### Calculo de dosis

- Se calcula la unidades de insulina con el siguiente calculo `f/(ValorRef/2)`, dando como resultado entre 1-3 unidades
- El valor final esperado se calcula como `f - (unidades de insulina * 50)`

### Tabla de valores

- **Planilla**: [link de drive](https://docs.google.com/spreadsheets/d/1eZSCC03KBxAKdVK58ssGJT3xnhAzRS-QLdvUbbb3vlo/edit?usp=sharing)

t|dt|Oi (valorRef)|Sensor de agua|f|e (Oi - f)|e previo|e diff (e - e previo) /tiempo|proporcional (e < 0)|derivativo con tiempo|OR|Unidades de insulina|siguiente dt|Valor final esperado
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
0|0|180|0|50|130|null|null|FALSO|FALSO|FALSO|0|1000|50
1000|1000|180|512|175|5|null|null|FALSO|FALSO|FALSO|0|500|175
1500|500|180|512|175|5|5|0|FALSO|FALSO|FALSO|0|500|175
2000|500|180|450|160|20|5|30|FALSO|FALSO|FALSO|0|500|160
2500|500|180|390|145|35|20|30|FALSO|FALSO|FALSO|0|500|145
3000|500|180|450|160|20|35|-30|FALSO|FALSO|FALSO|0|500|160
3500|500|180|512|175|5|20|-30|FALSO|FALSO|FALSO|0|500|175
4000|500|180|600|197|-17|5|-44|VERDADERO|VERDADERO|VERDADERO|2|17000|97
21000|17000|180|1023|300|-120|-17|-6,058823529|VERDADERO|FALSO|VERDADERO|3|17000|150
38000|17000|180|700|221|-41|-120|4,647058824|VERDADERO|FALSO|VERDADERO|2|17000|121
55000|17000|180|570|189|-9|-41|1,882352941|VERDADERO|FALSO|VERDADERO|2|17000|89
72000|17000|180|300|123|57|-9|3,882352941|FALSO|FALSO|FALSO|0|1000|123
73000|1000|180|275|117|63|57|6|FALSO|FALSO|FALSO|0|1000|117
74000|1000|180|250|111|69|63|6|FALSO|FALSO|FALSO|0|1000|111
75000|1000|180|200|99|81|69|12|FALSO|FALSO|FALSO|0|1000|99
76000|1000|180|150|87|93|81|12|FALSO|FALSO|FALSO|0|1000|87
77000|1000|180|150|87|93|93|0|FALSO|FALSO|FALSO|0|1000|87
78000|1000|180|500|172|8|93|-85|FALSO|VERDADERO|VERDADERO|1|17000|122
95000|17000|180|300|123|57|8|2,882352941|FALSO|FALSO|FALSO|0|1000|123

### Gráfico de tabla de valores

![Screenshot from 2022-11-16 19-34-45](https://user-images.githubusercontent.com/4097554/202314324-4bae1307-c992-43cc-b813-cdc4e346454f.png)

### Ver en consola de linux

```bash
stty -F /dev/ttyACM0 raw 9600
cat /dev/ttyACM0
```

### Imagenes

#### Valor normal (bajo) -> dt = 1000

![Screenshot from 2022-11-16 20-11-57](https://user-images.githubusercontent.com/4097554/202315198-e718d181-4add-46d5-a47a-aa3973871b22.png)

#### Valor alto (pasado de 180) -> activación por proporcional -> inyección de insulina

![Screenshot from 2022-11-16 20-12-52](https://user-images.githubusercontent.com/4097554/202315192-80f1d650-b889-4905-a8cc-27d3688629c6.png)

#### Valor alto (pasado de 180) -> espera post inyección por proporcional

![Screenshot from 2022-11-16 20-13-00](https://user-images.githubusercontent.com/4097554/202315189-8024528c-a466-4254-9db5-d825fb5f59b5.png)

#### Valor normal (medio-alto) -> dt = 500

![Screenshot from 2022-11-16 20-13-52](https://user-images.githubusercontent.com/4097554/202315184-b0e8b014-1a33-480c-9cd5-89aa3f6ab402.png)

#### Valor normal (bajo) -> dt = 1000

![Screenshot from 2022-11-16 20-13-56](https://user-images.githubusercontent.com/4097554/202315180-635b9a7f-166b-4c62-a710-9522a03fc6fb.png)

#### Valor normal (alto -> warning) con activación por derivativo

![Screenshot from 2022-11-16 20-14-02](https://user-images.githubusercontent.com/4097554/202315176-445b9b25-35d9-45ec-801d-0e4fb1214ab0.png)
![Screenshot from 2022-11-16 20-14-09](https://user-images.githubusercontent.com/4097554/202315171-51b2fdfd-1567-4181-84b9-d94fb5282136.png)

#### Valor normal (alto -> warning) con post activación derivativo (después de espera)

![Screenshot from 2022-11-16 20-15-21](https://user-images.githubusercontent.com/4097554/202315157-c0b17b1c-c3ff-442d-8935-86f1ec2c9d5c.png)
