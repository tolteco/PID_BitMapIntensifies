

########### PLANO ##############################

- Ler a imagem em BMP;
- Transformar para YUV;
- Armazenar em formato "pr�prio" (BMP com um dos bytes reservados marcando o sistema de cores? E alterando a ordem
   do armazenamento do mapa de bits?)
- Quantizar para 8 bits os canais UV (Continuar armazenando em 24 bits);
- Transformar para RGB;
- Criar paleta;
- Armazenar em BMP;


############ CONSIDERA��ES EXTRAS ##########################

- Formato MBT: "Extens�o do BMP", isto �, mesmo n�mero de campos mas possuem outros prop�sitos.

-- BfType = "MB"
-- BfSize = ColorSpace (0, RGB; 1, YUV; 2, HSV)
-- BfReser1 = Byte mais alto para bits do primeiro canal, depois do segundo canal;
-- BfReser2 = Byte mais alto para bits do terceiro canal, depois do quarto (Se n�o houver, 0);
--




