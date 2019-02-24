from django.db import models
import hashlib, datetime, random
import os

def get_image_path(instance,filename):
    return os.path.join('photos',str(instance.id),filename)




class Usuario(models.Model):
    Usuario=models.CharField(max_length=45)
    Contrasenia=models.CharField(max_length=45)
    Tipo=models.IntegerField(default=1)
    Nombre = models.CharField(max_length=45)
    Apellido = models.CharField(max_length=45)
    Correo = models.CharField(max_length=70)
    Telefono = models.CharField(max_length=10)
    Imagen=models.ImageField(blank=True,null=False)
    Genero=models.CharField(max_length=5)
    Fecha_reg=models.DateTimeField(auto_now_add=True)
    Direccion=models.TextField()
    Fecha_nac=models.DateTimeField()
    Estado=models.IntegerField(default=0)
    Membresia=models.IntegerField(default=0)
    class Meta:
        db_table='Usuario'

class Historial(models.Model):
    Descripcion=models.CharField(max_length=100)
    Accion=models.IntegerField()
    Admin=models.ForeignKey(Usuario,related_name='Admin_accion')
    Usuario=models.ForeignKey(Usuario,related_name='Usuario_afectado')
    class Meta:
        db_table='Historial'

class UserProfile(models.Model):
    user=models.OneToOneField(Usuario)
    activation_key = models.CharField(max_length=40, blank=True)
    key_expires = models.DateTimeField(default=datetime.date.today())

    def __str__(self):
        return self.user.username

    class Meta:
        verbose_name_plural = u'Perfiles de Usuario'


class Pais(models.Model):
    nombre=models.CharField(max_length=40)
    class Meta:
        db_table='Pais'


class Equipo(models.Model):
    Nombre=models.CharField(max_length=100)
    Fecha=models.IntegerField()
    Pais=models.ForeignKey(Pais,related_name='Pais',on_delete=models.PROTECT)
    Logo=models.ImageField(blank=True,null=False)
    class Meta:
        db_table='Equipo'

class Estadio(models.Model):
    Nombre = models.CharField(max_length=100)
    Fecha = models.DateTimeField(default=datetime.date.today)
    Pais = models.ForeignKey(Pais, related_name='PaisEstadio', on_delete=models.PROTECT)
    cantidad=models.IntegerField()
    Direccion = models.CharField(max_length=100)
    Estado = models.CharField(max_length=100)
    Foto = models.ImageField(blank=True, null=False)
    class Meta:
        db_table='Estadio'


class Jugador(models.Model):
    Nombre = models.CharField(max_length=100)
    Fecha_nac = models.DateTimeField(default=datetime.date.today)
    Fecha_inicio = models.DateTimeField(default=datetime.date.today)
    Fecha_fin = models.DateTimeField(default=datetime.date.today)
    Estado = models.CharField(max_length=100)
    Posicion = models.CharField(max_length=100)
    Pais = models.ForeignKey(Pais, related_name='PaisJugador', on_delete=models.PROTECT)
    Equipo = models.ForeignKey(Equipo, related_name='EquipoJugador', on_delete=models.PROTECT)
    class Meta:
        db_table='Jugador'
    def age(self):
        import datetime
        return int((datetime.date.today() - self.Fecha_nac.date()).days / 365.25)


class BitacoraJugador(models.Model):
    Fecha_inicio=models.DateTimeField()
    Fecha_fin=models.DateTimeField()
    Equipo = models.ForeignKey(Equipo, related_name='EquipoJugadorBitacora', on_delete=models.PROTECT)
    Jugador = models.ForeignKey(Jugador, related_name='JugadorBitacora', on_delete=models.PROTECT)

class DT(models.Model):
    Nombre = models.CharField(max_length=100)
    Fecha_nac = models.DateTimeField()
    Estado = models.CharField(max_length=100)
    Pais = models.ForeignKey(Pais, related_name='PaisDT', on_delete=models.PROTECT)
    Equipo = models.ForeignKey(Equipo, related_name='EquipoDT', on_delete=models.PROTECT)
    Foto = models.ImageField(blank=True, null=False)
    class Meta:
        db_table='DT'
    def age(self):
        import datetime
        return int((datetime.date.today() - self.Fecha_nac.date()).days / 365.25)

class BitacoraDT(models.Model):
    Fecha_inicio=models.DateTimeField()
    Fecha_fin=models.DateTimeField()
    Equipo = models.ForeignKey(Equipo, related_name='EquipoDTBitacora', on_delete=models.PROTECT)
    DT = models.ForeignKey(DT, related_name='DTBitacora', on_delete=models.PROTECT)
    class Meta:
        db_table='Bitacora_DT'


class Competencia(models.Model):
    Nombre = models.CharField(max_length=100)
    Anio = models.IntegerField()
    Tipo = models.CharField(max_length=100)
    Campeon = models.ForeignKey(Equipo, related_name='CampeonCompetencia', on_delete=models.PROTECT,null=True)
    Pais = models.ForeignKey(Pais, related_name='PaisCompetencia', on_delete=models.PROTECT)
    class Meta:
        db_table='Competencia'

class Historial_Competencia(models.Model):
    Equipo = models.ForeignKey(Equipo, related_name='EquipoHistorial', on_delete=models.PROTECT)
    Competencia = models.ForeignKey(Competencia, related_name='CompetenciaHistorial', on_delete=models.PROTECT)
    class Meta:
        db_table='Historial_Competencia'


class Partido(models.Model):
    Fecha=models.DateTimeField()
    Estadio=models.ForeignKey(Estadio, related_name='EstadioPais', on_delete=models.PROTECT)
    Asistencia=models.IntegerField(default=0)
    EquipoVisita = models.ForeignKey(Equipo, related_name='EquipoVisita', on_delete=models.PROTECT)
    EquipoLocal = models.ForeignKey(Equipo, related_name='EquipoLocal', on_delete=models.PROTECT)
    Competencia = models.ForeignKey(Competencia, related_name='CompetenciaPartido', on_delete=models.PROTECT)
    Resultado=models.CharField(max_length=100,default="0-0")
    Estado=models.CharField(max_length=100)



class Incidencias(models.Model):
    Minuto=models.IntegerField()
    Descripcion=models.CharField(max_length=100)
    Partido=models.ForeignKey(Partido, related_name='IncidenciaPartido', on_delete=models.PROTECT)
    Equipo = models.ForeignKey(Equipo, related_name='IncidenciaEquipo', on_delete=models.PROTECT)
    Jugador = models.ForeignKey(Jugador, related_name='JugadorIncidencia', on_delete=models.PROTECT)
    class Meta:
        db_table='Incidencia'


class Noticias(models.Model):
    Titulo=models.CharField(max_length=100)
    Cuerpo=models.CharField(max_length=600)
    Fecha= models.DateTimeField(auto_now_add=True)
    Equipo = models.ForeignKey(Equipo, related_name='NoticiaEquipo', on_delete=models.PROTECT)
    Empleado = models.ForeignKey(Usuario, related_name='EmpleadoNoticias', on_delete=models.PROTECT)
    Foto = models.ImageField(blank=True, null=False)
    class Meta:
        db_table="Noticias"

class Membre(models.Model):
    Fecha_inicio=models.DateTimeField(auto_now_add=True)
    Fecha_fin = models.DateTimeField()
    Usuario = models.ForeignKey(Usuario, related_name='MembresiaUsuario', on_delete=models.PROTECT)
    class Meta:
        db_table="Membresia"

class Seguir(models.Model):
    Usuario = models.ForeignKey(Usuario, related_name='UsuarioSeguir', on_delete=models.PROTECT)
    Equipo = models.ForeignKey(Equipo, related_name='EquipoSeguir', on_delete=models.PROTECT)
    class Meta:
        db_table="Seguir"








