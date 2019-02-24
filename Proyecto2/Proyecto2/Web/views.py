from io import BytesIO
from rest_framework import status
from rest_framework.decorators import api_view
from dbus import ValidationException
import json

from django.db import connection
from django.shortcuts import render, redirect, render_to_response,get_object_or_404
from django.template import RequestContext
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.platypus import Paragraph, TableStyle, Table

from Web.models import *
from django.db.models import Q
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4
from reportlab.lib.units import cm
from django.http import Http404


import datetime

from Web.serializers import *
from rest_framework import generics
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework.viewsets import GenericViewSet
from rest_framework.mixins import (RetrieveModelMixin, CreateModelMixin, ListModelMixin, RetrieveModelMixin)
from rest_framework import viewsets


from django.utils.encoding import force_bytes, force_text
import hashlib, datetime, random
from django.core.mail import send_mail
from django.utils import timezone
from django.http import HttpResponse
from django.core.files.storage import FileSystemStorage
from django.db.models import ProtectedError




#------------------------------------------------PAGINA DE INICIO-----------------------------------------
def home(request):
    #del request.session['user']
    if 'user' in request.session:
        us=Usuario.objects.filter(Usuario=request.session['user'])
        tipo=request.session['tipo']
        membresia=us[0].Membresia
        return render_to_response('Web/home.html', RequestContext(request, {'client': tipo,'Usuario':us[0],'Membresia':membresia}))
    else:
        return render_to_response('Web/home.html', RequestContext(request, {'client': 0}))

#-------------------------------------------------LOGIN--------------------------------------

def Login(request):
    if 'user' in request.session:
        return render_to_response('Web/home.html', RequestContext(request, {'client': request.session['tipo']}))
    return render(request,"Web/login.html")

#------------------------------------------------LOGOUT---------------------------------------------------------------------
def Logout(request):
    if 'user' in request.session:
        del request.session['user']
    return render_to_response('Web/home.html', RequestContext(request, {'client': 0}))

#---------------------------------------------MI CUENTA------------------------------------------------------
def MyAccount(request):
    if 'user' in request.session:
        user=Usuario.objects.filter(Usuario=request.session['user'])
        return render_to_response('Web/MyAccount.html', RequestContext(request, {'var': request.session['tipo'],'Usuario':user[0]}))
    return render_to_response('Web/home.html', RequestContext(request, {'client': 0}))

#-----------------------------------------------Foto de Perfil----------------------------------------
def ProfilePicture(request):
    if 'user' in request.session:
        user = Usuario.objects.filter(Usuario=request.session['user'])
        return render_to_response('Web/Picture.html', RequestContext(request, {'var': request.session['tipo'], 'Usuario': user[0]}))
    return render_to_response('Web/home.html', RequestContext(request, {'client': 0}))

#----------------------------------------------Editar foto de perfil------------------------------------------
def EditPicture(request):
    if request.method == "POST":
        imagen = request.FILES['imagen']
        fs=FileSystemStorage()
        filename=fs.save(imagen.name,imagen)
        user = Usuario.objects.filter(Usuario=request.session['user'])
        user.update(Imagen=imagen)
        return render_to_response('Web/Picture.html',RequestContext(request, {'var': request.session['tipo'], 'Usuario': user[0], 'Exito': True}))

    else:
        return redirect('/Web')
#----------------------------------------------Editar Cuenta--------------------------------------------------
def editAccount(request):
    if request.method=="POST":
        contra=request.POST['contrasenia']
        user = Usuario.objects.filter(Usuario=request.session['user'])
        nombre=request.POST['full-name']
        apellido=request.POST['apellido']
        correo=request.POST['email-address']
        telefono=request.POST['numero']
        genero=request.POST['genero']
        direccion=request.POST['direccion']
        fecha_nac=request.POST['fecha_nac']
        user.update(Contrasenia=contra,Nombre=nombre,Apellido=apellido,Correo=correo,Telefono=telefono,Genero=genero,Direccion=direccion,Fecha_nac=fecha_nac)
        return render_to_response('Web/MyAccount.html', RequestContext(request, {'var': request.session['tipo'], 'Usuario': user[0],'Exito':True}))

    else:
        return redirect('/Web')
#-----------------------------------------------VERIFICAR USUARIO----------------------------------------------
def auth_user(request):
    if 'user' in request.session:
        return render_to_response('Web/home.html', RequestContext(request, {'client': 1}))
    else:
        if request.method=="POST":
           usuario=request.POST['username']
           password=request.POST['password']
           user = Usuario.objects.filter(Usuario=usuario,Contrasenia=password)
           exist=user.exists()
           if exist == True:
                if user[0].Estado==1:
                    request.session['user']=user[0].Usuario
                    request.session['tipo']=(user[0].Tipo+1)
                    return  redirect('/Web')
                elif user[0].Estado==2:
                    return render_to_response('Web/login.html',
                                              RequestContext(request, {'congelado': True}))
                else:
                    return render_to_response('Web/login.html', RequestContext(request, {'estado': True,'invalida':False}))
           else:
               return  render_to_response('Web/login.html',RequestContext(request,{'invalid':True,'estado':False}))
        else:
            return redirect('/Web')
#-------------------------------------------------Eliminar Cuenta---------------------------------------------
def DeleteAccount(request):
    if 'user' in request.session:
        user=Usuario.objects.filter(Usuario=request.session['user'])
        user.delete()
        return redirect('/Web/Logout/')
    return redirect('/Web')

#-----------------------------------------------NUEVO USUARIO (ADMIN)------------------------------------------
def newUser(request):
    if 'user' in request.session:
        user=Usuario.objects.filter(Usuario=request.session['user'])
        return render_to_response('Web/newUser.html', RequestContext(request, {'var':request.session['tipo'],'Usuario':user[0]}))

    return redirect('/Web')
#----------------------------------------------Crear usuario desde Admin--------------------------------------------
def createUser(request):
    if 'user' in request.session:
        if request.method == "POST":
            contra = request.POST['contrasenia']
            contra2 = request.POST['contrasenia2']
            us = request.POST['username']
            if contra == contra2:
                user = Usuario.objects.filter(Usuario=us).exists()
                if user == True:
                    return render_to_response('Web/newUser.html', RequestContext(request, {'pass': False, 'use': True}))
                else:
                    nombre = request.POST['full-name']
                    apellido = request.POST['apellido']
                    correo = request.POST['email-address']
                    telefono = request.POST['numero']
                    genero = request.POST['genero']
                    direccion = request.POST['direccion']
                    fecha_nac = request.POST['fecha_nac']
                    imagen = request.FILES['imagen']
                    estado=1
                    razon=request.POST['Descripcion']
                    tipo=request.POST['tipo']
                    nuevo_user = Usuario(Nombre=nombre, Usuario=us, Contrasenia=contra, Tipo=tipo, Apellido=apellido,
                                         Correo=correo,
                                         Telefono=telefono, Imagen=imagen, Genero=genero, Direccion=direccion,
                                         Fecha_nac=fecha_nac,Estado=1)
                    nuevo_user.save()
                    actual_user=Usuario.objects.get(Usuario=request.session['user'])
                    c=connection.cursor()
                    try:
                        c.callproc("guardarHistorial",(razon,1,actual_user.id,nuevo_user.id))
                    finally:
                        c.close()
                    return render_to_response('Web/newUser.html', RequestContext(request, {'var':request.session['tipo'],'exito': True}))

            else:
                return render_to_response('Web/newUser.html', RequestContext(request, {'var':request.session['tipo'],'pass': True}))
        return redirect('/Web')
    return redirect('/Web')
#------------------------------------------------PAGINA DE REGISTRO-----------------------------------------
def register_user(request):
    return render(request,'Web/Register.html')

#------------------------------------------------VERIFICAR EL REGISTRO----------------------------------------
def new_user(request):
    if request.method=="POST":
        contra=request.POST['contrasenia']
        contra2 = request.POST['contrasenia2']
        us=request.POST['username']
        if contra==contra2:
            user=Usuario.objects.filter(Usuario=us).exists()
            if user == True:
                return render_to_response('Web/Register.html', RequestContext(request, {'pass': False,'use':True}))
            else:
                nombre=request.POST['full-name']
                apellido=request.POST['apellido']
                correo=request.POST['email-address']
                telefono=request.POST['numero']
                genero=request.POST['genero']
                direccion=request.POST['direccion']
                fecha_nac=request.POST['fecha_nac']
                imagen=request.FILES['imagen']
                nuevo_user=Usuario(Nombre=nombre,Usuario=us,Contrasenia=contra,Tipo=1,Apellido=apellido,Correo=correo,
                                   Telefono=telefono,Imagen=imagen,Genero=genero,Direccion=direccion,Fecha_nac=fecha_nac)
                nuevo_user.save()

                salt=hashlib.sha1(str(random.random())).hexdigest()[:5]
                activation_key=hashlib.sha1(salt+correo).hexdigest()
                key_expires=datetime.datetime.today()+datetime.timedelta(2)

                new_profile=UserProfile(user=nuevo_user,activation_key=activation_key,key_expires=key_expires)
                new_profile.save()

                email_subject="Verificar Cuenta"
                email_body = "Hola %s Gracias pro registrarte. Para activar tu cuenta da click en este link en menos de 48 horas: http://127.0.0.1:8000/Web/confirm/%s" % (
                us, activation_key)

                send_mail(email_subject, email_body, 'carloshernandez.chsegunda@gmail.com',
                          [correo], fail_silently=False)

                return HttpResponse("Por favor revisa tu correo para Finalizar el registro")

        else:
            return render_to_response('Web/Register.html', RequestContext(request, {'pass': True}))
    else:
        return redirect('/Web')



#-------------------------------------------VERIFICAR EMAIL--------------------------------------------------
def register_confirm(request,activation_key):
    user_profile=get_object_or_404(UserProfile,activation_key=activation_key)

    if user_profile.key_expires<timezone.now():
        return HttpResponse("Ha expirado la url")
    user=user_profile.user
    user.Estado=1
    user.save()
    user_profile.delete()
    return HttpResponse("tu cuenta ha sido verificada con exito")


#--------------------------------------------View Cambiar Estado-----------------------------------------------------------
def changeUser(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            user = Usuario.objects.filter(Usuario=request.session['user'])
            if request.method == "POST":
                buscar=request.POST['usuario']
                users=Usuario.objects.filter(Usuario__contains=buscar)
                return render_to_response('Web/changeEstado.html', RequestContext(request, {'var':request.session['tipo'],'Usuario':user[0],'buscando':True,'Users':users}))
            else:
                return render_to_response('Web/changeEstado.html', RequestContext(request, {'var':request.session['tipo'],'Usuario':user[0]}))
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

def viewUser(request,slug):

    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            user = Usuario.objects.get(id=slug)
            usuario=Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/Usuario.html', {'SearchUser': user,'Usuario':usuario,'var':request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

def EditUsers(request):
    if request.method=='POST':
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            razon = request.POST['description']
            actual_user = Usuario.objects.get(Usuario=request.session['user'])
            us=Usuario.objects.get(Usuario=request.POST['Usuario'])
            if 'ActivarUsuario' in request.POST:
                user = Usuario.objects.filter(Usuario=request.POST['Usuario'])
                user.update(Estado=1)
                c = connection.cursor()
                try:
                    c.callproc("guardarHistorial", (razon, 2, actual_user.id, us.id))
                finally:
                    c.close()
                return HttpResponse("Usuario Activado con exito")
            elif 'CongelarUsuario' in request.POST:
                user=Usuario.objects.filter(Usuario=request.POST['Usuario'])
                user.update(Estado=2)
                c = connection.cursor()
                try:
                    c.callproc("guardarHistorial", (razon, 3, actual_user.id, us.id))
                finally:
                    c.close()
                return HttpResponse("Usuario Congelado con exito")
            elif 'eliminarUsuario' in request.POST:
                user=Usuario.objects.filter(Usuario=request.POST['Usuario'])
                histo=Historial.objects.filter(Usuario=us.id)
                try:
                    histo.delete()
                    user.delete()
                except ProtectedError:
                    return HttpResponse("No se puede eliminar el usuario")
                return HttpResponse("Usuario eliminado con exito")
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------------------Pais--------------------------------------------------
def createPais(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/Pais.html',
                      {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------------Guardar Pais--------------------------------------------------
def savePais(request):
    if request.method == 'POST':
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            nombre=request.POST['nombre']
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            pais=Pais.objects.filter(nombre=nombre)
            encontrado=pais.exists()
            if not encontrado:
                nuevo_pais=Pais(nombre=nombre)
                nuevo_pais.save()
                return render(request, 'Web/Pais.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
            else:
                return render(request, 'Web/Pais.html',
                              {'Usuario': usuario, 'var': request.session['tipo'],'error':True})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------Busqueda Pais----------------------------------------------------------

def buscarPais(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method=='POST':
                buscar = request.POST['pais']
                pais = Pais.objects.filter(nombre__contains=buscar)
                return render_to_response('Web/buscarPais.html', RequestContext(request, {'var': request.session['tipo'],'Usuario': usuario,'buscando': True,'Pais': pais}))
            else:
                return render(request, 'Web/buscarPais.html',
                          {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------Ver Pais Seleccionado----------------------------------------------------
def viewPais(request,slug):

    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            pais = Pais.objects.get(id=slug)
            usuario=Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/verPais.html', {'searchPais': pais,'Usuario':usuario,'var':request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------Edita el Pais-------------------------------------------------------
def editPais(request):
    if request.method=='POST':
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            pais=Pais.objects.filter(id=request.POST['id'])
            nombre=request.POST['pais']
            if 'ActualizarPais' in request.POST:
                pais.update(nombre=nombre)
                return render(request, 'Web/verPais.html',
                              {'searchPais': pais[0], 'Usuario': usuario, 'var': request.session['tipo'],'editado':True})
            elif 'eliminarPais' in request.POST:
                try:
                    pais.delete()
                except ProtectedError:
                    return HttpResponse("No se puede eliminar el Pais")
                return HttpResponse("Pais eliminado con exito")
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------html equipo----------------------------------------------------------------------------
def addequipo(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            paises=Pais.objects.all()
            return render(request, 'Web/addEquipo.html',
                          {'Pais': paises, 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------crear equipo---------------------------------------------------------------------------------
def createEquipo(request):
    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                paises = Pais.objects.all()
                pais=request.POST['pais']
                nombre=request.POST['nombre']
                anio=request.POST['fecha']
                imagen = request.FILES['imagen']
                equi=Equipo.objects.filter(Nombre=nombre,Pais=pais)
                if not equi.exists():
                    pa=Pais.objects.get(id=pais)
                    equipo=Equipo(Nombre=nombre,Pais=pa,Fecha=anio,Logo=imagen)
                    equipo.save()
    
                    return render(request, 'Web/addEquipo.html',
                                  {'Pais': paises, 'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
                return render(request, 'Web/addEquipo.html',
                              {'Pais': paises, 'Usuario': usuario, 'var': request.session['tipo'],'use':True})
            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------Buscar Equipo-------------------------------------------------------------
def buscarEquipo(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                nombre = request.POST['equipo']
                equipo = Equipo.objects.filter(Nombre__contains=nombre)
                return render_to_response('Web/buscarEquipo.html', RequestContext(request, {'var': request.session['tipo'],
                                                                                          'Usuario': usuario,
                                                                                          'buscando': True, 'Equipo': equipo}))
            else:
                return render(request, 'Web/buscarEquipo.html',
                              {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------Ver equipo
def viewEquipo(request,slug):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            equipo = Equipo.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            pais=Pais.objects.all()
            return render(request, 'Web/verEquipo.html',
                      {'searchEquipo': equipo, 'Usuario': usuario, 'var': request.session['tipo'],'Pais':pais})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------Editar Equipo---------------------------------------------------------------
def editEquipo(request):
    if request.method=='POST':
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipo=Equipo.objects.filter(id=request.POST['id'])
            nombre=request.POST['nombre']
            fecha=request.POST['fecha']
            pa=request.POST['pais']
            pais=Pais.objects.get(id=pa)
            APais=Pais.objects.all()
            if 'actualizarEquipo' in request.POST:
                if len(request.FILES)!=0:
                    imagen = request.FILES['imagen']
                    fs = FileSystemStorage()
                    filename = fs.save(imagen.name, imagen)
                    equipo.update(Nombre=nombre, Fecha=fecha, Pais=pais, Logo=imagen)
                else:
                    equipo.update(Nombre=nombre,Fecha=fecha,Pais=pais)
                return render(request, 'Web/verEquipo.html',
                              {'searchEquipo': equipo[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Pais': APais, 'editado': True})
            elif 'eliminarEquipo' in request.POST:
                try:
                    equipo.delete()
                except ProtectedError:
                    return HttpResponse("No se puede eliminar el Equipo")
                return HttpResponse("Equipo eliminado con exito")
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------html carga masiva------------------------------------------------
def cargaEquipo(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/cargaEquipo.html',
                      { 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return  redirect('/Web')

#--------------------------------------subir carga masiva-------------------------------------------------
def uploadCargaEquipo(request):

    if request.method=='POST':
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        try:
            csv_file=request.FILES['archivo']

            file_data=csv_file.read().decode("ISO-8859-3")
            lines=file_data.split('\n')
            numero=0;
            for line in lines:
                fields=line.split(';')
                if (not fields[0]=="") and  numero!=0:
                    equipo=fields[0]
                    anio=fields[1]
                    pais=fields[2]
                    pais=pais.replace('\r',"")
                    paisM=Pais.objects.filter(nombre=pais)
                    encontrado=paisM.exists()
                    if not encontrado:
                        paisM=Pais(nombre=pais)
                        paisM.save()
                    paisM=Pais.objects.get(nombre=pais)
                    equipoTemp = Equipo.objects.filter(Nombre=equipo,Pais=paisM)
                    encontrado = equipoTemp.exists()
                    if not encontrado:
                        nuevo_equipo = Equipo(Nombre=equipo, Logo='./Default.png',Fecha=anio,Pais=paisM)
                        nuevo_equipo.save()
                numero=numero+1
            return render(request, 'Web/cargaEquipo.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
        except Exception as e:
            return render(request, 'Web/cargaEquipo.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'errorException': True,'mensaje':repr(e)})
    return redirect('/Web')


#-------------------------------------------Pagina HTML estadio--------------------------------------------------
def addEstadio(request):

    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            paises = Pais.objects.all()
            return render(request, 'Web/addEstadio.html',
                          {'Pais': paises, 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------Guardar Estadio-----------------------------------------------------
def saveEstadio(request):

    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                paises = Pais.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                nombre=request.POST['nombre']
                cantidad=request.POST['cantidad']
                direccion=request.POST['direccion']
                estado=request.POST['estado']
                fecha=request.POST['Fecha']
                imagen=request.FILES['imagen']
                pai=request.POST['pais']
                estadioTemp=Estadio.objects.filter(Nombre=nombre,Pais=pai)
                if not estadioTemp.exists():
                    pais=Pais.objects.get(id=pai)
                    nuevoEstadio=Estadio(Nombre=nombre,cantidad=cantidad,Fecha=fecha,Direccion=direccion,Estado=estado,Foto=imagen,Pais=pais)
                    nuevoEstadio.save()
                    return render(request, 'Web/addEstadio.html',
                                  {'Pais': paises, 'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
                return render(request, 'Web/addEstadio.html',
                                  {'Pais': paises, 'Usuario': usuario, 'var': request.session['tipo'], 'error': True})
            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#----------------------------------------------Pagina HTML de buscar Estadio-----------------------------------------------
def buscarEstadio(request):

    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                nombre = request.POST['estadio']
                estadio = Estadio.objects.filter(Nombre__contains=nombre)
                return render_to_response('Web/buscarEstadio.html', RequestContext(request, {'var': request.session['tipo'],
                                                                                          'Usuario': usuario,
                                                                                          'buscando': True, 'Estadio': estadio}))
            else:
                return render(request, 'Web/buscarEstadio.html',
                              {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#----------------------------------------Pagina para ver estadio-------------------------------------------------------------
def viewEstadio(request,slug):

    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            estadio = Estadio.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            pais=Pais.objects.all()
            return render(request, 'Web/verEstadio.html',
                      {'searchEstadio': estadio, 'Usuario': usuario, 'var': request.session['tipo'],'Pais':pais})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------Editar o Eliminar Estadio-----------------------------------------------------------
def editEstadio(request):

    if request.method=='POST':
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            estadio=Estadio.objects.filter(id=request.POST['id'])
            nombre = request.POST['nombre']
            cantidad = request.POST['cantidad']
            direccion = request.POST['direccion']
            estado = request.POST['estado']
            pai = request.POST['pais']
            fecha = request.POST['Fecha']
            pais=Pais.objects.get(id=pai)
            APais=Pais.objects.all()
            if 'actualizarEstadio' in request.POST:
                if len(request.FILES)!=0:
                    imagen = request.FILES['imagen']
                    fs = FileSystemStorage()
                    filename = fs.save(imagen.name, imagen)
                    estadio.update(Nombre=nombre, Fecha=fecha, Pais=pais, Foto=imagen,cantidad=cantidad,Direccion=direccion,Estado=estado)
                else:
                    estadio.update(Nombre=nombre, Fecha=fecha, Pais=pais,cantidad=cantidad,Direccion=direccion,Estado=estado)
                return render(request, 'Web/verEstadio.html',
                              {'searchEstadio': estadio[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Pais': APais, 'editado': True})
            elif 'eliminarEstadio' in request.POST:
                try:
                    estadio.delete()
                except ProtectedError:
                    return HttpResponse("No se puede eliminar el Estadio")
                return HttpResponse("Estadio eliminado con exito")
        return  redirect('/Error')
    return redirect('/Web')
#-----------------------------------------------------Pagina carga Estadios---------------------------------------------
def cargaEstadio(request):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/cargaEstadio.html',
                      { 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return  redirect('/Web')


# ---------------------------------------------------------guardar carga masiva estadio-----------------------------------------
def uploadCargaEstadio(request):

    if request.method == 'POST':
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.session['tipo']==1 or request.session['tipo']==3:
            try:
                csv_file = request.FILES['archivo']


                file_data = csv_file.read().decode("ISO-8859-3")
                lines = file_data.split('\n')
                numero = 0;
                for line in lines:
                    fields = line.split(';')
                    if (not fields[0] == "") and numero != 0:
                        pais = fields[0]
                        nombre = fields[1]
                        fecha = fields[2]
                        date = datetime.datetime.strptime(fecha, '%d/%m/%Y').date()

                        capacidad = fields[3]
                        direccion=fields[4]
                        estado=fields[5]
                        paisM = Pais.objects.filter(nombre=pais)
                        encontrado = paisM.exists()
                        if not encontrado:
                            paisM = Pais(nombre=pais)
                            paisM.save()
                        paisM = Pais.objects.get(nombre=pais)
                        estadioTemp=Estadio.objects.filter(Nombre=nombre,Pais=paisM)
                        encontrado = estadioTemp.exists()
                        if not encontrado:
                            nuevo_Estadio=Estadio(Nombre=nombre,Fecha=date,Pais=paisM,cantidad=capacidad,Direccion=direccion,Estado=estado,Foto='./Default.png')
                            nuevo_Estadio.save()
                    numero = numero + 1
                return render(request, 'Web/cargaEstadio.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
            except Exception as e:
                return render(request, 'Web/cargaEstadio.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'errorException': True,
                               'mensaje': repr(e)})
        return render(request, 'Web/error.html',{})

    return redirect('/Web')



#----------------------------------------------------------Pagina HTML para Jugador---------------------------------------
def addJugador(request):

    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            paises = Pais.objects.all()
            equipos=Equipo.objects.all()
            return render(request, 'Web/addJugador.html',
                          {'Pais': paises, 'Equipo':equipos,'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------------------Guardar Jugador------------------------------------------------------
def saveJugador(request):

    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                paises = Pais.objects.all()
                equipos=Equipo.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                nombre=request.POST['nombre']
                fecha_nac=request.POST['fechanac']
                fecha_inicio = request.POST['fechainicio']
                fecha_fin = request.POST['fechafin']
                estado=request.POST['estado']
                pai=request.POST['pais']
                equip=request.POST['equipo']
                posicion=request.POST['posicion']
                jugadorTemp=Jugador.objects.filter(Nombre=nombre,Equipo=equip)
                if not jugadorTemp.exists():
                    pais=Pais.objects.get(id=pai)
                    equipo=Equipo.objects.get(id=equip)
                    nuevoJugador=Jugador(Nombre=nombre,Fecha_nac=fecha_nac,Fecha_inicio=fecha_inicio,Fecha_fin=fecha_fin,Estado=estado,Pais=pais,Equipo=equipo,Posicion=posicion)
                    nuevoJugador.save()
                    nuevoJugador=Jugador.objects.get(Nombre=nombre)
                    bitacora=BitacoraJugador(Fecha_inicio=fecha_inicio,Fecha_fin=fecha_fin,Jugador=nuevoJugador,Equipo=equipo)
                    bitacora.save()
                    return render(request, 'Web/addJugador.html',
                                  {'Pais': paises, 'Equipo': equipos, 'Usuario': usuario,
                                   'var': request.session['tipo'],'exito':True})
                return render(request, 'Web/addJugador.html',
                              {'Pais': paises, 'Equipo': equipos, 'Usuario': usuario, 'var': request.session['tipo'],'error':True})
            return render(request, 'Web/error.html',{})
    return redirect('/Web')


#------------------------------------------------Pagina html para buscar Jugador--------------------------------------------
def buscarJugador(request):

    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                nombre = request.POST['jugador']
                jugador = Jugador.objects.filter(Nombre__contains=nombre)
                return render_to_response('Web/buscarJugador.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario,
                                                                   'buscando': True, 'Jugador': jugador}))
            else:
                return render(request, 'Web/buscarJugador.html',
                              {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------pagina para mostrar el jugador-----------------------------------
def viewJugador(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            jugador = Jugador.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            pais=Pais.objects.all()
            return render(request, 'Web/verJugador.html',
                      {'searchJugador': jugador, 'Usuario': usuario, 'var': request.session['tipo'],'Pais':pais})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------pagina para mostrar el jugador y equipo-----------------------------------
def viewEquipoJugador(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            jugador = Jugador.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipo=Equipo.objects.all()
            return render(request, 'Web/verEquipoJugador.html',
                      {'searchJugador': jugador, 'Usuario': usuario, 'var': request.session['tipo'],'Equipo':equipo})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')
#-------------------------------------------------Editar datos de jugador---------------------------------------------
def editJugador(request):

    if request.method=='POST':
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            jugador=Jugador.objects.filter(id=request.POST['id'])
            nombre = request.POST['nombre']
            posicion = request.POST['posicion']
            estado = request.POST['estado']
            fecha_nac = request.POST['fechanac']
            fecha_fin = request.POST['fechafin']
            pai = request.POST['pais']
            pais=Pais.objects.get(id=pai)
            APais=Pais.objects.all()
            if 'ActualizarJugador' in request.POST:
                jugador.update(Nombre=nombre, Fecha_nac=fecha_nac,Fecha_fin=fecha_fin,Pais=pais,Estado=estado,Posicion=posicion)
                return render(request, 'Web/verJugador.html',
                              {'searchJugador': jugador[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Pais': APais, 'editado': True})
            elif 'eliminarJugador' in request.POST:
                try:
                    jugador.delete()
                except ProtectedError:
                    return HttpResponse("No se puede eliminar el Jugador")
                return HttpResponse("Jugador eliminado con exito")
        return  redirect('/Error')
    return redirect('/Web')

#-------------------------------------------------Editar equipo jugador------------------------------------------------
def editEquipoJugador(request):

    if request.method == 'POST':
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            jugador = Jugador.objects.filter(id=request.POST['id'])
            fecha_nac = request.POST['fechainicio']
            fecha_fin = request.POST['fechafin']
            equip=request.POST['equipo']
            equipo=Equipo.objects.get(id=equip)
            totaequo=Equipo.objects.all()
            jugador.update(Fecha_nac=fecha_nac, Fecha_fin=fecha_fin, Equipo=equipo)
            bitacora=BitacoraJugador(Fecha_inicio=fecha_nac,Fecha_fin=fecha_fin,Jugador=jugador[0],Equipo=equipo)
            bitacora.save()
            return render(request, 'Web/verEquipoJugador.html',
                              {'searchJugador': jugador[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Equipo': totaequo, 'editado': True})

        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------------ver bitacora de un jugador----------------------------------------
def viewBitacora(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            jugador = Jugador.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            bitacora=BitacoraJugador.objects.filter(Jugador=jugador)
            return render(request, 'Web/verBitacora.html',
                      {'Usuario': usuario, 'var': request.session['tipo'],'Bitacora':bitacora,'Jugador':jugador})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------------pagina de subir carga de Jugador---------------------------------
def cargarJugador(request):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/cargaJugador.html',
                      { 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return  redirect('/Web')

#------------------------------------------------subir carga masiva--------------------------------------------
def uploadCargaJugador(request):
    if request.method == 'POST':
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.session['tipo']==1 or request.session['tipo']==3:
            try:
                csv_file = request.FILES['archivo']


                file_data = csv_file.read().decode("ISO-8859-3")
                lines = file_data.split('\n')
                numero = 0;
                for line in lines:
                    fields = line.split(';')
                    if (not fields[0] == "") and numero != 0:
                        nombre = fields[0]
                        fecha=fields[1]
                        fecha = fecha.replace('\r',"")
                        fecha_nac=datetime.datetime.strptime(fecha, '%d/%m/%Y').date()
                        pai = fields[2]
                        pai=pai.replace('\r',"")
                        posicion = fields[3]
                        paequipo=fields[4]
                        paequipo = paequipo.replace('\r', "")
                        equip=fields[5]
                        equip = equip.replace('\r', "")

                        fecha = fields[6]
                        fecha = fecha.replace('\r',"")
                        fecha_inicio=datetime.datetime.strptime(fecha, '%d/%m/%Y').date()
                        fecha = fields[7]
                        fecha = fecha.replace('\r',"")
                        fecha_fin = datetime.datetime.strptime(fecha, '%d/%m/%Y').date()

                        pais=Pais.objects.filter(nombre=pai)
                        if not pais.exists():
                            pais=Pais(nombre=pai)
                            pais.save()
                        pais=Pais.objects.get(nombre=pai)
                        paisequipo=Pais.objects.get(nombre=paequipo)
                        equipo=Equipo.objects.filter(Nombre=equip,Pais=paisequipo)
                        if not equipo.exists():
                            equipo=Equipo(Nombre=equip,Fecha=1998,Pais=paisequipo,Logo='./Default.png')
                            equipo.save()
                        equipo=Equipo.objects.get(Nombre=equip,Pais=paisequipo)

                        juga=Jugador.objects.filter(Nombre=nombre)
                        if juga.exists():
                            if not juga[0].Equipo==equipo:
                                juga.update(Fecha_inicio=fecha_inicio,Fecha_fin=fecha_fin,Equipo=equipo)
                                bitacora = BitacoraJugador(Fecha_inicio=fecha_inicio, Fecha_fin=fecha_fin,
                                                           Equipo=equipo, Jugador=juga[0])
                                bitacora.save()
                        else:
                            nuevo_Jugador=Jugador(Nombre=nombre,Fecha_nac=fecha_nac,Fecha_inicio=fecha_inicio,Fecha_fin=fecha_fin,Estado='Activo',Posicion=posicion,Equipo=equipo,Pais=pais)
                            nuevo_Jugador.save()
                            nuevo_Jugador=Jugador.objects.get(Nombre=nombre)
                            bitacora=BitacoraJugador(Fecha_inicio=fecha_inicio,Fecha_fin=fecha_fin,Equipo=equipo,Jugador=nuevo_Jugador)
                            bitacora.save()
                    numero = numero + 1
                return render(request, 'Web/cargaJugador.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
            except Exception as e:
                return render(request, 'Web/cargaJugador.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'errorException': True,
                               'mensaje': repr(e)})
        return render(request, 'Web/error.html',{})

    return redirect('/Web')


#-----------------------------------------------pagina html para crear DT---------------------------------------
def addDT(request):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            paises = Pais.objects.all()
            equipos=Equipo.objects.all()
            return render(request, 'Web/addDT.html',
                          {'Pais': paises, 'Equipo':equipos,'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#----------------------------------------------------GUARDAR DT--------------------------------------------
def saveDT(request):
    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                paises = Pais.objects.all()
                equipos=Equipo.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                nombre=request.POST['nombre']
                fecha_nac=request.POST['fechanac']
                fecha_inicio = request.POST['fechainicio']
                fecha_fin = request.POST['fechafin']
                estado=request.POST['estado']
                pai=request.POST['pais']
                imagen=request.FILES['imagen']
                equip=request.POST['equipo']
                dtTemp=DT.objects.filter(Nombre=nombre,Equipo=equip)
                if not dtTemp.exists():
                    pais=Pais.objects.get(id=pai)
                    equipo=Equipo.objects.get(id=equip)
                    nuevoDT=DT(Nombre=nombre,Fecha_nac=fecha_nac,Estado=estado,Pais=pais,Equipo=equipo,Foto=imagen)
                    nuevoDT.save()
                    nuevoDT=DT.objects.get(Nombre=nombre)
                    bitacora=BitacoraDT(Fecha_inicio=fecha_inicio,Fecha_fin=fecha_fin,DT=nuevoDT,Equipo=equipo)
                    bitacora.save()
                    return render(request, 'Web/addDT.html',
                                  {'Pais': paises, 'Equipo': equipos, 'Usuario': usuario,
                                   'var': request.session['tipo'],'exito':True})
                return render(request, 'Web/addDT.html',
                              {'Pais': paises, 'Equipo': equipos, 'Usuario': usuario, 'var': request.session['tipo'],'error':True})
            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------pagina para buscar DT-----------------------------------
def buscarDT(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                nombre = request.POST['dt']
                dt = DT.objects.filter(Nombre__contains=nombre)
                return render_to_response('Web/buscarDT.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario,
                                                                   'buscando': True, 'DT': dt}))
            else:
                return render(request, 'Web/buscarDT.html',
                              {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------------------cargarDT-------------------------------------------

def viewDT(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            dt = DT.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            pais=Pais.objects.all()
            return render(request, 'Web/verDT.html',
                      {'searchDT': dt, 'Usuario': usuario, 'var': request.session['tipo'],'Pais':pais})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------------------editar DT-----------------------------------------------

def editDT(request):
    if request.method=='POST':
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            dt = DT.objects.filter(id=request.POST['id'])
            nombre = request.POST['nombre']
            estado = request.POST['estado']
            fecha=request.POST['fechanac']
            pai = request.POST['pais']
            pais = Pais.objects.get(id=pai)
            APais = Pais.objects.all()
            if 'actualizarDT' in request.POST:
                if len(request.FILES)!=0:
                    imagen = request.FILES['imagen']
                    fs = FileSystemStorage()
                    filename = fs.save(imagen.name, imagen)
                    dt.update(Nombre=nombre, Fecha_nac=fecha, Pais=pais, Foto=imagen,Estado=estado)
                else:
                    dt.update(Nombre=nombre, Fecha_nac=fecha, Pais=pais,Estado=estado)
                return render(request, 'Web/verDT.html',
                              {'searchDT': dt[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Pais': APais, 'editado': True})
            elif 'eliminarDT' in request.POST:
                try:
                    dt.delete()
                except ProtectedError:
                    return HttpResponse("No se puede eliminar el DT")
                return HttpResponse("DT eliminado con exito")
        return  redirect('/Error')
    return redirect('/Web')

#-------------------------------------------------pagina para editar equipo------------------------------------
def viewEquipoDT(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            dt = DT.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipo=Equipo.objects.all()
            return render(request, 'Web/verEquipoDT.html',
                      {'searchDT': dt, 'Usuario': usuario, 'var': request.session['tipo'],'Equipo':equipo})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------------------cambiar al dt de equipo-------------------------------------

def editEquipoDT(request):
    if request.method == 'POST':
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            dt = DT.objects.filter(id=request.POST['id'])
            fecha_nac = request.POST['fechainicio']
            fecha_fin = request.POST['fechafin']
            equip=request.POST['equipo']
            equipo=Equipo.objects.get(id=equip)
            totaequo=Equipo.objects.all()
            dt.update(Equipo=equipo)
            bitacora=BitacoraDT(Fecha_inicio=fecha_nac,Fecha_fin=fecha_fin,DT=dt[0],Equipo=equipo)
            bitacora.save()
            return render(request, 'Web/verEquipoDT.html',
                              {'searchDT': dt[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Equipo': totaequo, 'editado': True})

        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------------PAGINA HTML PARA VER BITACORA------------------------------------
def viewBitacoraDT(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            dt = DT.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            bitacora=BitacoraDT.objects.filter(DT=dt)
            return render(request, 'Web/verBitacoraDT.html',
                      {'Usuario': usuario, 'var': request.session['tipo'],'Bitacora':bitacora,'DT':dt})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#----------------------------------------------pagina para subir carga de DT-------------------------------------
def cargarDT(request):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/cargarDT.html',
                      { 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return  redirect('/Web')

#-------------------------------------------------subir carga masiva de DT----------------------------------------
def uploadCargaDT(request):

    if request.method == 'POST':
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.session['tipo']==1 or request.session['tipo']==3:
            try:
                csv_file = request.FILES['archivo']

                file_data = csv_file.read().decode("ISO-8859-3")
                lines = file_data.split('\n')
                numero = 0;
                for line in lines:
                    fields = line.split(';')
                    if (not fields[0] == "") and numero != 0:
                        nombre = fields[0]
                        fecha=fields[1]
                        fecha = fecha.replace('\r',"")
                        fecha_nac=datetime.datetime.strptime(fecha, '%d/%m/%Y').date()
                        pai = fields[2]
                        pai=pai.replace('\r',"")
                        estado = fields[3]
                        paequipo=fields[4]
                        paequipo=paequipo.replace('\r',"")
                        equip=fields[5]
                        equip=equip.replace('\r',"")

                        fecha = fields[6]
                        fecha = fecha.replace('\r',"")
                        fecha_inicio=datetime.datetime.strptime(fecha, '%d/%m/%Y').date()
                        fecha = fields[7]
                        fecha = fecha.replace('\r',"")
                        fecha_fin = datetime.datetime.strptime(fecha, '%d/%m/%Y').date()

                        pais=Pais.objects.filter(nombre=pai)
                        if not pais.exists():
                            pais=Pais(nombre=pai)
                            pais.save()
                        pais=Pais.objects.get(nombre=pai)
                        paisequipo=Pais.objects.get(nombre=paequipo)
                        equipo=Equipo.objects.filter(Nombre=equip,Pais=paisequipo)
                        if not equipo.exists():
                            equipo=Equipo(Nombre=equip,Fecha=1998,Pais=paisequipo)
                            equipo.save()
                        equipo=Equipo.objects.get(Nombre=equip,Pais=paisequipo)

                        dt=DT.objects.filter(Nombre=nombre)
                        if dt.exists():
                            if not dt[0].Equipo==equipo:
                                dt.update(Equipo=equipo)
                                bitacora = BitacoraDT(Fecha_inicio=fecha_inicio, Fecha_fin=fecha_fin,
                                                           Equipo=equipo, DT=dt[0])
                                bitacora.save()
                        else:
                            nuevo_dt=DT(Nombre=nombre,Fecha_nac=fecha_nac,Estado=estado,Equipo=equipo,Pais=pais,Foto='./Default.png')
                            nuevo_dt.save()
                            nuevo_dt=DT.objects.get(Nombre=nombre)
                            bitacora=BitacoraDT(Fecha_inicio=fecha_inicio,Fecha_fin=fecha_fin,Equipo=equipo,DT=nuevo_dt)
                            bitacora.save()
                    numero = numero + 1
                return render(request, 'Web/cargarDT.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
            except Exception as e:
                return render(request, 'Web/cargarDT.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'errorException': True,
                               'mensaje': repr(e)})
        return render(request, 'Web/error.html',{})

    return redirect('/Web')


#---------------------------------------------------pagina para crear competencias---------------------------------
def addCompeticion(request):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            paises = Pais.objects.all()
            return render(request, 'Web/addCompetencia.html',
                          {'Pais': paises,'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------------guardar Competencia--------------------------------------------
def saveCompetencia(request):

    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                paises = Pais.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                nombre=request.POST['nombre']
                anio=request.POST['fecha']
                tipo=request.POST['tipo']
                pai=request.POST['pais']
                pais=Pais.objects.get(id=pai)
                competenciaTemp=Competencia.objects.filter(Nombre=nombre,Anio=anio,Pais=pais)
                if not competenciaTemp.exists():
                    competencia=Competencia(Nombre=nombre,Anio=anio,Pais=pais,Tipo=tipo)
                    competencia.save()
                    return render(request, 'Web/addCompetencia.html',
                                  {'Pais': paises, 'Usuario': usuario,
                                   'var': request.session['tipo'],'exito':True})
                return render(request, 'Web/addCompetencia.html',
                              {'Pais': paises,  'Usuario': usuario, 'var': request.session['tipo'],'error':True})
            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------------pagina para buscar Competencias---------------------------------
def buscarCompetencia(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                nombre = request.POST['competencia']
                competencia = Competencia.objects.filter(Nombre__contains=nombre)
                return render_to_response('Web/buscarCompetencia.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario,
                                                                   'buscando': True, 'Competencia': competencia}))
            else:
                return render(request, 'Web/buscarCompetencia.html',
                              {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------------pagina para editar competencia---------------------------------------------
def viewCompetencia(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            competencia = Competencia.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            pais=Pais.objects.all()
            return render(request, 'Web/verCompetencia.html',
                      {'searchCompetencia': competencia, 'Usuario': usuario, 'var': request.session['tipo'],'Pais':pais})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------------- Editar Competencia------------------------------------------
def editCompetencia(request):

    if request.method=='POST':
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            competencia = Competencia.objects.filter(id=request.POST['id'])
            nombre = request.POST['nombre']
            tipo = request.POST['tipo']
            fecha=request.POST['fecha']
            pai = request.POST['pais']
            pais = Pais.objects.get(id=pai)
            APais = Pais.objects.all()
            if 'actualizarCompetencia' in request.POST:
                competencia.update(Nombre=nombre, Anio=fecha, Pais=pais,Tipo=tipo)
                return render(request, 'Web/verCompetencia.html',
                              {'searchCompetencia': competencia[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Pais': APais, 'editado': True})
            elif 'eliminarCompetencia' in request.POST:
                try:
                    competencia.delete()
                except ProtectedError:
                    return HttpResponse("No se puede eliminar la Competencia")
                return HttpResponse("Competencia eliminada con exito")
        return  redirect('/Error')
    return redirect('/Web')


#-------------------------------------------------Pagina para agregar equipos a la competencion---------------
def viewEquipoCompeticion(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            competencia = Competencia.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipos=Equipo.objects.all()
            return render(request, 'Web/agregarEquipos.html',
                      {'searchCompetencia': competencia, 'Usuario': usuario, 'var': request.session['tipo'],'Equipo':equipos})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------------Agregar el Equipo a la Competencia---------------------------


def equipoCompetencia(request):
    if request.method=='POST':
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            competencia = Competencia.objects.filter(id=request.POST['id'])
            nombre = request.POST['id']
            equipo=request.POST['equipo']
            equipos=Equipo.objects.all()
            tempEquipo=Equipo.objects.get(id=equipo)
            tempLiga=Competencia.objects.get(id=nombre)
            tempHistorial=Historial_Competencia.objects.filter(Equipo=tempEquipo,Competencia=tempLiga)
            if not tempHistorial.exists():
                historial=Historial_Competencia(Equipo=tempEquipo,Competencia=tempLiga)
                historial.save()
                return render(request, 'Web/agregarEquipos.html',
                              {'searchCompetencia': competencia[0], 'Usuario': usuario, 'var': request.session['tipo'],
                               'Equipo': equipos, 'editado': True})
            return render(request, 'Web/agregarEquipos.html',
                          {'searchCompetencia': competencia[0], 'Usuario': usuario, 'var': request.session['tipo'],
                           'Equipo': equipos, 'existe': True})
        return  redirect('/Error')
    return redirect('/Web')


#---------------------------------------------------- ver los equipos de una competencia:---------------------------------
def viewCompetenciaEquipo(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            competencia = Competencia.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            historial=Historial_Competencia.objects.filter(Competencia=competencia)
            return render(request, 'Web/verCompeticionHistorial.html',
                      {'Usuario': usuario, 'var': request.session['tipo'],'Competicion':competencia,'Historial':historial})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#----------------------------------------------------- pagina para escoger campeon------------------------------------------
def agregarCampeon(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            competencia = Competencia.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            historial=Historial_Competencia.objects.filter(Competencia=competencia)
            return render(request, 'Web/agregarCampeon.html',
                      {'Usuario': usuario, 'var': request.session['tipo'],'Competicion':competencia,'Historial':historial})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------------- guardar campeon-------------------------------------------------
def setCampeon(request):
    if request.method=='POST':
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            competencia = Competencia.objects.filter(id=request.POST['id'])
            historial=Historial_Competencia.objects.filter(Competencia=competencia)
            equip=request.POST['equipo']
            equipo=Equipo.objects.get(id=equip)
            competencia.update(Campeon=equipo)
            competencia=Competencia.objects.get(id=request.POST['id'])
            return render(request, 'Web/agregarCampeon.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Competicion': competencia,
                           'Historial': historial,'exito':True})

        return  redirect('/Error')
    return redirect('/Web')

#-----------------------------------------------------pagina para cargar competencia---------------------
def cargarCompetencia(request):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/cargarCompetencia.html',
                      { 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return  redirect('/Web')

#----------------------------------------------------subir archivo de Competencia--------------------------------
def uploadCargaCompetencia(request):
    if request.method == 'POST':
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.session['tipo']==1 or request.session['tipo']==3:
            try:
                csv_file = request.FILES['archivo']


                file_data = csv_file.read().decode("ISO-8859-3")
                lines = file_data.split('\n')
                numero = 0;
                for line in lines:
                    fields = line.split(';')
                    if (not fields[0] == "") and numero != 0:
                        compe=fields[0]
                        compe=compe.replace('\r',"")
                        anio=fields[1]
                        anio=anio.replace('\r',"")
                        tipo=fields[2]
                        tipo=tipo.replace('\r',"")
                        campeon=fields[3]
                        campeon=campeon.replace('\r',"")
                        pai=fields[4]
                        pai=pai.replace('\r',"")
                        paiequipo=fields[5]
                        paiequipo=paiequipo.replace('\r',"")
                        equip=fields[6]
                        equip=equip.replace('\r',"")

                        paisequipo=Pais.objects.filter(nombre=paiequipo)
                        if not paisequipo.exists():
                            paisequipo=Pais(nombre=paiequipo)
                            paisequipo.save()
                        paiequipo=Pais.objects.get(nombre=paiequipo)

                        pais=Pais.objects.filter(nombre=pai)
                        if not pais.exists():
                            pais=Pais(nombre=pai)
                            pais.save()
                        pais=Pais.objects.get(nombre=pai)

                        equipoParticipo=Equipo.objects.filter(Nombre=equip,Pais=paiequipo)
                        if not equipoParticipo.exists():
                            equipoParticipo=Equipo(Nombre=equip,Fecha=1998,Pais=paiequipo,Logo='./Default.png')
                            equipoParticipo.save()
                        equipoParticipo=Equipo.objects.get(Nombre=equip,Pais=paiequipo)

                        competicion=Competencia.objects.filter(Nombre=compe,Anio=anio,Tipo=tipo,Pais=pais)
                        equipoCampeon=Equipo.objects.get(Nombre=campeon)
                        if not competicion.exists():
                            competicion=Competencia(Nombre=compe,Anio=anio,Tipo=tipo,Pais=pais,Campeon=equipoCampeon)
                            competicion.save()
                        competicion=Competencia.objects.get(Nombre=compe,Anio=anio,Tipo=tipo,Pais=pais)

                        historial=Historial_Competencia.objects.filter(Equipo=equipoParticipo,Competencia=competicion)
                        if not historial.exists():
                            historial=Historial_Competencia(Equipo=equipoParticipo,Competencia=competicion)
                            historial.save()
                        historial=Historial_Competencia.objects.get(Equipo=equipoParticipo,Competencia=competicion)
                    numero = numero + 1
                return render(request, 'Web/cargarCompetencia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
            except Exception as e:
                return render(request, 'Web/cargarCompetencia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'errorException': True,
                               'mensaje': repr(e)})
        return render(request, 'Web/error.html',{})

    return redirect('/Web')


#-------------------------------------------------pagina para agregar partidos-------------------------------------
def addPartido(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            estadio=Estadio.objects.all()
            equipo=Equipo.objects.all()
            competencia=Competencia.objects.all()
            return render(request, 'Web/addPartido.html',
                          {'Equipo': equipo,'Estadio':estadio, 'Usuario': usuario, 'var': request.session['tipo'],'Competencia':competencia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------guardar Partidos---------------------------------------------------
def savePartido(request):
    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                equipos = Equipo.objects.all()
                estadios=Estadio.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                fecha=request.POST['fecha']
                equipoVisita=Equipo.objects.get(id=request.POST['equipoVisita'])
                equipoLocal=Equipo.objects.get(id=request.POST['equipoLocal'])
                estado=request.POST['estado']
                resultado=request.POST['resultado']
                estadio=request.POST['estadio']
                asistencia=request.POST['asistencia']
                tempEstadio=Estadio.objects.get(id=estadio)
                partidoTemp=Partido.objects.filter(Fecha=fecha,EquipoVisita=equipoVisita,EquipoLocal=equipoLocal)
                competencia=Competencia.objects.get(id=request.POST['competencia'])
                competencias=Competencia.objects.all()
                if not partidoTemp.exists():
                    partido=Partido(Fecha=fecha,Competencia=competencia,EquipoVisita=equipoVisita,EquipoLocal=equipoLocal,Estadio=tempEstadio,Asistencia=asistencia,Resultado=resultado,Estado=estado)
                    partido.save()
                    return render(request, 'Web/addPartido.html',
                                  {'Competencia':competencias,'Equipo': equipos, 'Estadio': estadios, 'Usuario': usuario,
                                   'var': request.session['tipo'],'exito':True})

                return render(request, 'Web/addPartido.html',
                              {'Competencia':competencias,'Equipo': equipos, 'Estadio': estadios, 'Usuario': usuario,
                               'var': request.session['tipo'],'error':True})
            return render(request, 'Web/error.html',{})
    return redirect('/Web')


#---------------------------------------------pagina html para buscar--------------------------------------------------------
def buscarPartido(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                nombre = request.POST['fecha']
                paises=Pais.objects.all()
                pais=Pais.objects.get(id=request.POST['pais'])
                comp=Competencia.objects.filter(Pais=pais)
                partido = Partido.objects.filter(Competencia__in=comp,Fecha=nombre)
                print(partido)
                return render_to_response('Web/buscarPartido.html',
                                          RequestContext(request, {'Pais':paises,'var': request.session['tipo'],
                                                                   'Usuario': usuario,
                                                                   'buscando': True, 'Partido': partido}))
            else:
                pais=Pais.objects.all()
                return render(request, 'Web/buscarPartido.html',
                              {'Usuario': usuario, 'var': request.session['tipo'],'Pais':pais})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')


#---------------------------------------------Pagina para editar un partido---------------------------------------------
def viewPartido(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            competencias = Competencia.objects.all()
            equipos = Equipo.objects.all()
            estadios = Estadio.objects.all()
            partido = Partido.objects.get(id=slug)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/verPartido.html',
                          {'Equipo': equipos, 'Estadio': estadios, 'Usuario': usuario, 'var': request.session['tipo'],
                           'Competencia': competencias,'searchPartido':partido})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------editar Partido------------------------------------------------------
def editPartido(request):
    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                partido=Partido.objects.filter(id=request.POST['id'])
                equipos = Equipo.objects.all()
                estadios=Estadio.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                fecha=request.POST['fecha']
                equipoVisita=Equipo.objects.get(id=request.POST['equipoVisita'])
                equipoLocal=Equipo.objects.get(id=request.POST['equipoLocal'])
                estado=request.POST['estado']
                resultado=request.POST['resultado']
                estadio=request.POST['estadio']
                asistencia=request.POST['asistencia']
                tempEstadio=Estadio.objects.get(id=estadio)
                competencia=Competencia.objects.get(id=request.POST['competencia'])
                competencias=Competencia.objects.all()
                if 'actualizarPartido' in request.POST:
                    partido.update(Fecha=fecha,Competencia=competencia,EquipoVisita=equipoVisita,EquipoLocal=equipoLocal,Estadio=tempEstadio,Asistencia=asistencia,Resultado=resultado,Estado=estado)
                    return render(request, 'Web/verPartido.html',
                                  {'Equipo': equipos, 'Estadio': estadios, 'Usuario': usuario,
                                   'var': request.session['tipo'],
                                   'Competencia': competencias, 'searchPartido': partido[0],'editado':True})
                elif 'eliminarPartido' in request.POST:
                    try:
                        partido.delete()
                    except ProtectedError:
                        return HttpResponse("No se puede eliminar el Partido")
                    return HttpResponse("Partido eliminado con exito")


            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------pagina para agregar Incidencias---------------------------------------
def viewAgregarIncidencia(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            partido = Partido.objects.get(id=slug)
            equipos = Equipo.objects.filter(Q(id=partido.EquipoLocal.id)|Q(id=partido.EquipoVisita.id))
            jugador = Jugador.objects.filter(Equipo__in=equipos)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/verIncidencia.html',
                          {'Equipo': equipos, 'Usuario': usuario, 'var': request.session['tipo'],
                           'Jugador': jugador,'searchPartido':partido})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------guardar Incidencia------------------------------------------------------
def saveIncidencia(request):
    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                partido=Partido.objects.get(id=request.POST['id'])
                equipos = Equipo.objects.all()
                jugadores=Jugador.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                equipo=Equipo.objects.get(id=request.POST['equipo'])
                minuto=request.POST['minuto']
                descripcion=request.POST['descripcion']
                jugador=Jugador.objects.get(id=request.POST['jugador'])
                if partido.Estado=="En Curso":
                    incidencia=Incidencias(Minuto=minuto,Jugador=jugador,Equipo=equipo,Descripcion=descripcion,Partido=partido)
                    incidencia.save()
                    return render(request, 'Web/verIncidencia.html',
                                  {'Jugador':jugadores,'Equipo': equipos,'Usuario': usuario,
                                   'var': request.session['tipo'],'exito':True,'searchPartido':partido})
                return render(request, 'Web/verIncidencia.html',
                              {'Jugador': jugadores, 'Equipo': equipos, 'Usuario': usuario,
                               'var': request.session['tipo'], 'error': True, 'searchPartido': partido})

            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------ver las incidencias-------------------------------------------------------
def viewIncidencia(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            partido = Partido.objects.get(id=slug)
            incidencia=Incidencias.objects.filter(Partido=partido)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/showIncidencia.html',
                      {'Usuario': usuario, 'var': request.session['tipo'],'Incidencia':incidencia,'Partido':partido})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#----------------------------------------------pagina para ver carga masiva--------------------------------------------
def cargarIncidencia(request):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/cargarIncidencia.html',
                      { 'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return  redirect('/Web')

#-----------------------------------------------subir carga de incidencias----------------------------------------------
def uploadCargaIncidencia(request):
    if request.method == 'POST':
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.session['tipo']==1 or request.session['tipo']==3:
            try:
                csv_file = request.FILES['archivo']


                file_data = csv_file.read().decode("ISO-8859-3")
                lines = file_data.split('\n')
                numero = 0;
                for line in lines:
                    if line:
                        fields = line.split(';')
                        if (not fields[1] == " ") and numero != 0 and (not fields[2]==""):

                            date = fields[0]
                            date = date.replace('\r', "")
                            fecha = datetime.datetime.strptime(date, '%d/%m/%Y').date()

                            paises=fields[1]
                            paises=paises.replace('\r',"")

                            paisEstadio=Pais.objects.filter(nombre=paises)
                            if not paisEstadio.exists():
                                paisEstadio=Pais(nombre=paises)
                                paisEstadio.save()
                            paisEstadio=Pais.objects.get(nombre=paises)

                            nomEstadio=fields[2]
                            nomEstadio=nomEstadio.replace('\r',"")

                            estadio2=Estadio.objects.filter(Nombre=nomEstadio)
                            if not estadio2.exists():
                                estadio2=Estadio(Nombre=nomEstadio,Pais=paisEstadio,Direccion="Sin",cantidad=1000,Estado="Activo",Foto='./Default.png')
                                estadio2.save()
                            estadio=Estadio.objects.get(Nombre=nomEstadio)

                            estado=fields[3]
                            estado=estado.replace('\r',"")



                            paiLocal=fields[5]
                            paiLocal=paiLocal.replace('\r',"")
                            paisLocal=Pais.objects.filter(nombre=paiLocal)
                            if not paisLocal.exists():
                                paisLocal=Pais(nombre=paiLocal)
                                paisLocal.save()
                            paisLocal=Pais.objects.get(nombre=paiLocal)

                            equipLo=fields[6]
                            equipLo=equipLo.replace('\r',"")
                            equipoLocal=Equipo.objects.filter(Nombre=equipLo)
                            if not equipoLocal.exists():
                                equipoLocal=Equipo(Nombre=equipLo,Fecha=1998,Pais=paisLocal,Logo='./Default.png')
                                equipoLocal.save()
                            equipoLocal=Equipo.objects.get(Nombre=equipLo)

                            paiVisita = fields[7]
                            paiVisita = paiVisita.replace('\r', "")
                            paisVisita = Pais.objects.filter(nombre=paiVisita)
                            if not paisVisita.exists():
                                paisVisita = Pais(nombre=paiVisita)
                                paisVisita.save()
                            paisVisita = Pais.objects.get(nombre=paiVisita)

                            equipVi = fields[8]
                            equipVi = equipVi.replace('\r', "")
                            equipoVi= Equipo.objects.filter(Nombre=equipVi)
                            if not equipoVi.exists():
                                equipoVi = Equipo(Nombre=equipVi, Fecha=1998, Pais=paisVisita, Logo='./Default.png')
                                equipoVi.save()
                            equipoVi = Equipo.objects.get(Nombre=equipVi)

                            anioC=fields[15]
                            anioC=anioC.replace('\r',"")

                            nombCompe=fields[14]
                            nombCompe=nombCompe.replace('\r',"")

                            competencia=Competencia.objects.filter(Nombre=nombCompe,Anio=anioC)
                            if not competencia.exists():
                                competencia=Competencia(Nombre=nombCompe,Anio=anioC,Pais=paisVisita,Tipo="Liga")
                                competencia.save()
                            competenciass=Competencia.objects.filter(Nombre=nombCompe,Anio=anioC)
                            competencia=competenciass[0]



                            if estado!="Sin Iniciar" and estado!="Suspendido":
                                asistencia = fields[4]
                                asistencia = asistencia.replace('\r', "")

                                resultado=fields[9]
                                resultado=resultado.replace('\r',"")

                                partido = Partido.objects.filter(EquipoLocal=equipoLocal, EquipoVisita=equipoVi,
                                                                 Fecha=fecha)
                                if not partido.exists():
                                    partido = Partido(EquipoLocal=equipoLocal, EquipoVisita=equipoVi, Fecha=fecha,
                                                      Asistencia=asistencia,
                                                      Resultado=resultado, Estado=estado, Estadio=estadio,
                                                      Competencia=competencia)
                                    partido.save()
                                else:
                                    partido.update(Resultado=resultado)
                                partido = Partido.objects.get(EquipoLocal=equipoLocal, EquipoVisita=equipoVi,
                                                              Fecha=fecha)

                                tipo=fields[10]
                                tipo=tipo.replace('\r',"")

                                minuto=fields[11]
                                minuto=minuto.replace('\r',"")

                                equiPInci=fields[12]
                                equiPInci=equiPInci.replace('\r',"")

                                if equiPInci=="Local":
                                    equipoIncidencia=equipoLocal
                                else:
                                    equipoIncidencia=equipoVi

                                nombJ=fields[13]
                                nombJ=nombJ.replace('\r',"")
                                jugadore=Jugador.objects.filter(Nombre=nombJ)
                                if not jugadore.exists():
                                    jugadore=Jugador(Nombre=nombJ,Pais=paisVisita,Equipo=equipoIncidencia,Estado='Activo',Posicion='Medio')
                                    jugadore.save()
                                jugadore=Jugador.objects.get(Nombre=nombJ)

                                incidencia=Incidencias(Minuto=minuto,Descripcion=tipo,Partido=partido,Equipo=equipoIncidencia,Jugador=jugadore)
                                incidencia.save()
                            else:
                                partido = Partido.objects.filter(EquipoLocal=equipoLocal, EquipoVisita=equipoVi,
                                                             Fecha=fecha)
                                if not partido.exists():
                                    partido = Partido(EquipoLocal=equipoLocal, EquipoVisita=equipoVi, Fecha=fecha, Estado=estado, Estadio=estadio,
                                                      Competencia=competencia)
                                    partido.save()

                        numero = numero + 1
                return render(request, 'Web/cargarIncidencia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'exito': True})
            except Exception as e:
                return render(request, 'Web/cargarIncidencia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'errorException': True,
                               'mensaje': repr(e)})
        return render(request, 'Web/error.html',{})

    return redirect('/Web')

#------------------------------------------pagina para agregar noticias--------------------------------------------------
def addNoticia(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipo=Equipo.objects.all()
            return render(request, 'Web/addNoticia.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Equipo':equipo})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------guardar Noticias

def saveNoticia(request):
    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                equipo = Equipo.objects.all()
                titulo=request.POST['titulo']
                cuerpo=request.POST['noticia']
                equip=Equipo.objects.get(id=request.POST['equipo'])
                imagen=request.FILES['imagen']
                noticia=Noticias(Empleado=usuario,Equipo=equip,Titulo=titulo,Cuerpo=cuerpo,Foto=imagen)
                noticia.save()
                return render(request, 'Web/addNoticia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Equipo': equipo,'exito':True})
            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------- buscar Noticia----------------------------------------------
def buscarNoticia(request):
    if 'user' in request.session:
        if request.session['tipo'] == 1 or request.session['tipo'] == 3:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                nombre = request.POST['fecha']
                noticia=Noticias.objects.filter(Fecha__contains=nombre,Empleado=usuario).order_by('-Fecha')
                return render_to_response('Web/buscarNoticia.html',
                                          RequestContext(request, {'Noticia':noticia,'var': request.session['tipo'],
                                                                   'Usuario': usuario,
                                                                   'buscando': True}))
            else:
                return render(request, 'Web/buscarNoticia.html',
                              {'Usuario': usuario, 'var': request.session['tipo']})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------pagina ver noticia--------------------------------------
def viewNoticia(request,slug):
    if 'user' in request.session:
        if request.session['tipo']==1 or request.session['tipo']==3:
            noticia = Noticias.objects.get(id=slug)
            equipos=Equipo.objects.all()
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            return render(request, 'Web/verNoticia.html',
                      {'Usuario': usuario, 'var': request.session['tipo'],'noticia':noticia,'Equipo':equipos})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------editar Noticicia-----------------------------------------------
def editNoticia(request):

    if request.method=='POST':
        if 'user' in request.session:
            if request.session['tipo'] == 1 or request.session['tipo'] == 3:
                noticia=Noticias.objects.filter(id=request.POST['id'])
                equipos = Equipo.objects.all()
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                titulo=request.POST['titulo']
                cuerpo=request.POST['noticia']
                equipo=Equipo.objects.get(id=request.POST['equipo'])
                if 'actualizarNoticia' in request.POST:
                    if len(request.FILES) != 0:
                        imagen = request.FILES['imagen']
                        fs = FileSystemStorage()
                        filename = fs.save(imagen.name, imagen)
                        noticia.update(Titulo=titulo,Foto=imagen,Equipo=equipo,Cuerpo=cuerpo,Empleado=usuario)
                    else:
                        noticia.update(Titulo=titulo, Equipo=equipo, Cuerpo=cuerpo, Empleado=usuario)
                    return render(request, 'Web/verNoticia.html',
                                  {'Usuario': usuario, 'var': request.session['tipo'], 'noticia': noticia[0],
                                   'Equipo': equipos,'editado':True})
                elif 'eliminarNoticia' in request.POST:
                    try:
                        noticia.delete()
                    except ProtectedError:
                        return HttpResponse("No se puede eliminar la Noticia")
                    return HttpResponse("Noticia eliminada con exito")


            return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------ver todas las noticias------------------------------------------
def AllNoticies(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        noticia=Noticias.objects.all().order_by('-Fecha')
        return render(request, 'Web/allNoticias.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Noticia':noticia,'Membresia':usuario.Membresia})
    return redirect('/Web')

#---------------------------------------------Noticias Filtradas---------------------------------------------
def filterNoticias(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        raw_sql="SELECT * FROM Equipo INNER JOIN Seguir ON Equipo.id=Seguir.Equipo_id WHERE Seguir.Usuario_id="+str(usuario.id)
        equipo=Equipo.objects.raw(raw_sql)
        noticia=Noticias.objects.filter(Equipo__in=equipo).order_by('-Fecha')
        return render(request, 'Web/allNoticias.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Noticia':noticia,'Membresia':usuario.Membresia})
    return redirect('/Web')


#---------------------------------------------pagina de membresia-------------------------------------------
def Membresia(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        return render(request, 'Web/Membresia.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia})
    return redirect('/Web')

#---------------------------------------------pagina para comprar Membresia----------------------------------
def comprarMembresia(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            if 'SuscribirseAhora' in request.POST:
                return render(request, 'Web/comprarMembresia.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia})
            elif 'EliminarSubscripcion' in request.POST:
                usuario = Usuario.objects.filter(Usuario=request.session['user'])
                usuario.update(Membresia=0)
                usuario = Usuario.objects.get(Usuario=request.session['user'])
                return render(request, 'Web/Membresia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
    return redirect('/Web')

#--------------------------------------------Guardar la membresia------------------------------------------------
def saveMembresia(request):
    if 'user' in request.session:
        if request.method=='POST':
            usuario = Usuario.objects.filter(Usuario=request.session['user'])
            usuario.update(Membresia=1)
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            fecha_fin=datetime.datetime.today()+datetime.timedelta(30)
            print(fecha_fin)
            membresia=Membre(Fecha_fin=fecha_fin,Usuario=usuario)
            membresia.save()
            return render(request, 'Web/comprarMembresia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,'exito':True})
    return redirect('/Web')

#-------------------------------------------- pagina para buscar partidos Usuario---------------------------------
def partidosUser(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method!='POST':
            return render(request, 'Web/userPartidos.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia})
        else:
            tipo=request.POST['buscar']
            if tipo=='Todos':
                partidos=Partido.objects.all().order_by('-Fecha')
            elif tipo=='Favoritos':
                raw_sql = "SELECT * FROM Equipo INNER JOIN Seguir ON Equipo.id=Seguir.Equipo_id WHERE Seguir.Usuario_id=" + str(
                    usuario.id)
                equipo = Equipo.objects.raw(raw_sql)

                partidos = Partido.objects.filter(Q(EquipoLocal__in=equipo)|Q(EquipoVisita__in=equipo)).order_by('-Fecha')
            else:
                partidos=Partido.objects.filter(Estado=tipo).order_by('-Fecha')
            return render(request, 'Web/userPartidos.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                           'buscando':True,'Partido':partidos})

    return redirect('/Web')

#--------------------------------------------- html buscar partidos---------------------------------------------------

def equiposUser(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            nombre = request.POST['equipo']
            equipo = Equipo.objects.filter(Nombre__contains=nombre)
            return render_to_response('Web/userBuscarEquipo.html', RequestContext(request, {'var': request.session['tipo'],
                                                                                      'Usuario': usuario,
                                                                                      'buscando': True, 'Equipo': equipo,'Membresia':usuario.Membresia}))
        else:
            return render(request, 'Web/userBuscarEquipo.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia})
    return redirect('/Web')

#-----------------------------------------------html para seguir equipos--------------------------------------
def seguirEquipo(request,slug):
    if 'user' in request.session:
        equipo = Equipo.objects.get(id=slug)
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        seguir=Seguir.objects.filter(Usuario=usuario,Equipo=equipo)
        mandar=seguir.exists()
        return render(request, 'Web/followEquipo.html',
                      {'searchEquipo': equipo, 'Usuario': usuario, 'var': request.session['tipo'],
                       'Membresia':usuario.Membresia,'Siguiendo':mandar})
    return redirect('/Web')

#------------------------------------------------seguir o dejar de seguir--------------------------------------
def editSeguirEquipo(request):
    if request.method=='POST':
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        equipo=Equipo.objects.get(id=request.POST['id'])
        if 'seguirEquipo' in request.POST:
            seguir=Seguir(Usuario=usuario,Equipo=equipo)
            seguir.save()
            return render(request, 'Web/followEquipo.html',
                          {'searchEquipo': equipo, 'Usuario': usuario, 'var': request.session['tipo'],
                           'Membresia': usuario.Membresia, 'Siguiendo': True,'editado':True})
        elif 'dejarSeguirEquipo' in request.POST:
            seguir=Seguir.objects.filter(Usuario=usuario,Equipo=equipo)
            seguir.delete()
            return render(request, 'Web/followEquipo.html',
                          {'searchEquipo': equipo, 'Usuario': usuario, 'var': request.session['tipo'],
                           'Membresia': usuario.Membresia, 'Siguiendo': False,'eliminado':True})
    return redirect('/Web')

#--------------------------------------------pagina de Jugares Tecnico Equipo-------------------------------------
def jugadoresTecnicoEquipo(request):

    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            nombre = request.POST['equipo']
            equipo = Equipo.objects.filter(id=nombre)
            jugador=Jugador.objects.filter(Equipo__in=equipo)
            tecnico=DT.objects.filter(Equipo__in=equipo)
            equipos=Equipo.objects.all()
            return render_to_response('Web/jugadoresTecnico.html', RequestContext(request, {'var': request.session['tipo'],
                                                'Usuario': usuario,'buscando': True, 'Equipo': equipos,
                                                'Membresia':usuario.Membresia,'Jugador':jugador,'Tecnico':tecnico}))
        else:
            equipos=Equipo.objects.all()
            return render(request, 'Web/jugadoresTecnico.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia,
                           'Equipo':equipos,'buscando':True})
    return redirect('/Web')

#---------------------------------------------pagina de jugadores tecnico edad-----------------------------------
def jugadoresTecnicosEdad(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            try:
                edad = int(request.POST.get("edad"))
            except ValueError:
                raise 0
            jugador=Jugador.objects.all().order_by("-Fecha_nac")
            tecnico=DT.objects.all().order_by("-Fecha_nac")
            equipos=Equipo.objects.all()
            return render_to_response('Web/jugadoresTecnicoEdad.html', RequestContext(request, {'var': request.session['tipo'],
                                                'Usuario': usuario,'buscando': True, 'Equipo': equipos,
                                                'Membresia':usuario.Membresia,'Jugador':jugador,'Tecnico':tecnico,'Edad':edad}))
        else:
            equipos=Equipo.objects.all()
            return render(request, 'Web/jugadoresTecnicoEdad.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia,
                           'Equipo':equipos})
    return redirect('/Web')

#---------------------------------------------pagina de jugadores tecnico edad Menor-----------------------------------
def jugadoresTecnicosEdadM(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            try:
                edad = int(request.POST.get("edad"))
            except ValueError:
                raise 0
            jugador=Jugador.objects.all().order_by("-Fecha_nac")
            tecnico=DT.objects.all().order_by("-Fecha_nac")
            equipos=Equipo.objects.all()
            return render_to_response('Web/jugadoresTecnicoEdad.html', RequestContext(request, {'var': request.session['tipo'],
                                                'Usuario': usuario,'buscandoM': True, 'Equipo': equipos,
                                                'Membresia':usuario.Membresia,'Jugador':jugador,'Tecnico':tecnico,'Edad':edad}))
        else:
            equipos=Equipo.objects.all()
            return render(request, 'Web/jugadoresTecnicoEdad.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia,
                           'Equipo':equipos})
    return redirect('/Web')

#--------------------------------------------- pagina de equipos en competenicion-------------------------------------------
def equiposCompeticionC(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        competencias=Competencia.objects.all()
        if request.method == 'POST':
            competencia=Competencia.objects.get(id=request.POST['competencia'])
            historial=Historial_Competencia.objects.filter(Competencia=competencia)
            return render_to_response('Web/equiposCompeticionC.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True, 'Historial': historial,
                                                               'Membresia': usuario.Membresia,'Competencia':competencias}))
        else:
            return render(request, 'Web/equiposCompeticionC.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                           'Competencia':competencias})
    return redirect('/Web')

#---------------------------------------------pagina de equipos por pais--------------------------------------------
def equiposPorPais(request):

    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        paises=Pais.objects.all()
        if request.method == 'POST':
            pais=Pais.objects.get(id=request.POST['pais'])
            equipo=Equipo.objects.filter(Pais=pais)
            return render_to_response('Web/equipoPorPais.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True, 'Equipo': equipo,
                                                               'Membresia': usuario.Membresia,'Pais':paises}))
        else:
            return render(request, 'Web/equipoPorPais.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                           'Pais':paises})
    return redirect('/Web')

#---------------------------------------------pagina para anioos de antiguedad----------------------------------------
def equiposAnios(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            try:
                anio = int(request.POST.get("anio"))
            except ValueError:
                raise 0
            anio=2018-anio
            equipo=Equipo.objects.filter(Fecha=anio)
            return render_to_response('Web/equiposAnios.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True, 'Equipo': equipo,
                                                               'Membresia': usuario.Membresia}))
        else:
            return render(request, 'Web/equiposAnios.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,})
    return redirect('/Web')

#---------------------------------------------pagina de estadios por pais-------------------------------------------
def estadiosPorPais(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        paises=Pais.objects.all()
        if request.method == 'POST':
            pais=Pais.objects.get(id=request.POST['pais'])
            estadio=Estadio.objects.filter(Pais=pais)
            return render_to_response('Web/estadioPorPais.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True, 'Estadio': estadio,
                                                               'Membresia': usuario.Membresia,'Pais':paises}))
        else:
            return render(request, 'Web/estadioPorPais.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                           'Pais':paises})
    return redirect('/Web')

#--------------------------------------------- pagina estadios por capacidad----------------------------------------
def estadiosPorCapacidad(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            try:
                capacidad = int(request.POST.get("capacidad"))
            except ValueError:
                raise 0
            estadio=Estadio.objects.filter(cantidad__lte=capacidad).order_by('-cantidad')
            return render_to_response('Web/estadiosPorCapacidad.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True, 'Estadio': estadio,
                                                               'Membresia': usuario.Membresia}))
        else:
            return render(request, 'Web/estadiosPorCapacidad.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,})
    return redirect('/Web')

#---------------------------------------------- pagina de historial de equipo----------------------------------------
def historialDeEquipo(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        equipos = Equipo.objects.all()
        if request.method == 'POST':
            equipo =Equipo.objects.get(id=request.POST['equipo'])
            partido=Partido.objects.filter(Q(EquipoLocal=equipo)|Q(EquipoVisita=equipo)).order_by('-Fecha')
            return render_to_response('Web/historialDeEquipo.html', RequestContext(request, {'var': request.session['tipo'],
                                                'Usuario': usuario,'buscando': True, 'Equipo': equipos,
                                                'Membresia':usuario.Membresia,'Partido':partido}))
        else:
            return render(request, 'Web/historialDeEquipo.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia,
                           'Equipo':equipos,'buscando':True})
    return redirect('/Web')

#---------------------------------------------- pagina para ver bitacora de jugador o tecnico------------------------
def EquipoJugadorTecnico(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        jugadores=Jugador.objects.all()
        tecnicos=DT.objects.all()
        if request.method == 'POST':
            jugador=Jugador.objects.filter(id=request.POST['jugador'])
            tecnico=DT.objects.filter(id=request.POST['tecnico'])

            bitacoraJ=BitacoraJugador.objects.filter(Jugador__in=jugador)
            bitacoraT=BitacoraDT.objects.filter(DT__in=tecnico)

            return render_to_response('Web/EquipoJugadorTecnico.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True,
                                                               'Membresia': usuario.Membresia,
                                                               'Jugador':bitacoraJ,'Tecnico':bitacoraT,
                                                               'Jugadores':jugadores,'Tecnicos':tecnicos}))
        else:
            return render(request, 'Web/EquipoJugadorTecnico.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                           'Jugadores':jugadores, 'Tecnicos': tecnicos})
    return redirect('/Web')

#----------------------------------------------- Jugadores con mas incidencias en una competicion----------------------
def JugadoresCompeticionIncidencia(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        competencia=Competencia.objects.values('Nombre').distinct()
        if request.method == 'POST':

            cantidad = request.POST.get("cantidad")

            nombre=request.POST['competencia']
            c = connection.cursor()
            c.execute("SELECT Jugador.Nombre,Count(Jugador.Nombre) FROM Incidencia "+
                            "INNER JOIN Jugador ON Incidencia.jugador_id=Jugador.id "+
                            "INNER JOIN web_partido ON Incidencia.Partido_id=web_partido.id "+
                            "INNER JOIN Competencia ON web_partido.competencia_id=Competencia.id "+
                            "WHERE Competencia.Nombre='"+nombre+"' "+
                            "GROUP BY(Jugador.Nombre) "+
                            "HAVING COUNT(Jugador.Nombre)>"+cantidad)
            row=c.fetchall()
            return render_to_response('Web/JugadoresCompeticionIncidencia.html', RequestContext(request, {'var': request.session['tipo'],
                                                'Usuario': usuario,'buscandoM': True, 'Competencia': competencia,
                                                'Membresia':usuario.Membresia,'Jugador':row,'buscando':True}))
        else:
            return render(request, 'Web/JugadoresCompeticionIncidencia.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia,
                           'Competencia':competencia})
    return redirect('/Web')


#----------------------------------------------- Jugadores con mas incidencias en una Competencia Anio----------------------
def JugadoresCompeticionIncidenciaAnio(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        competencia=Competencia.objects.all()
        if request.method == 'POST':

            cantidad = request.POST.get("cantidad")

            nombre=request.POST['competencia']
            c = connection.cursor()
            c.execute("SELECT Jugador.Nombre,Count(Jugador.Nombre) FROM Incidencia "+
                            "INNER JOIN Jugador ON Incidencia.jugador_id=Jugador.id "+
                            "INNER JOIN web_partido ON Incidencia.Partido_id=web_partido.id "+
                            "INNER JOIN Competencia ON web_partido.competencia_id=Competencia.id "+
                            "WHERE Competencia.id="+nombre+
                            " GROUP BY(Jugador.Nombre) "+
                            "HAVING COUNT(Jugador.Nombre)>"+cantidad)
            row=c.fetchall()
            return render_to_response('Web/JugadoresCompeticionIncidenciaAnio.html', RequestContext(request, {'var': request.session['tipo'],
                                                'Usuario': usuario,'buscandoM': True, 'Competencia': competencia,
                                                'Membresia':usuario.Membresia,'Jugador':row,'buscando':True}))
        else:
            return render(request, 'Web/JugadoresCompeticionIncidenciaAnio.html',
                          {'Usuario': usuario, 'var': request.session['tipo'],'Membresia':usuario.Membresia,
                           'Competencia':competencia})
    return redirect('/Web')

#------------------------------------------------competencias ganadas----------------------------------------------
def competenciasGanandas(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        equipos=Equipo.objects.all()
        if request.method == 'POST':
            nombre=request.POST['equipo']
            c = connection.cursor()
            c.execute("SELECT Equipo.Nombre,COUNT(Equipo.Nombre) FROM Equipo "+
                        "INNER JOIN Competencia ON Equipo.id=Competencia.campeon_id "+
                        " WHERE Equipo.Nombre='"+nombre+"' "+
                        "GROUP BY Equipo.Nombre")
            row = c.fetchall()
            return render_to_response('Web/competenciasGanandas.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True, 'Equipo': equipos,
                                                               'Membresia': usuario.Membresia,'Data':row}))
        else:
            return render(request, 'Web/competenciasGanandas.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                           'Equipo':equipos})
    return redirect('/Web')

#---------------------------------------------------partidos por anio----------------------------------------
def partidoPorAnio(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        if request.method == 'POST':
            anio = request.POST.get("anio")
            c = connection.cursor()
            c.execute("SELECT web_partido.Fecha,web_partido.Resultado,Equipo.Nombre,\"b\".Nombre,Estadio.Nombre FROM web_partido "+
                            " INNER JOIN Equipo ON web_partido.equipolocal_id=Equipo.id "+
                            " INNER JOIN Equipo \"b\" ON web_partido.equipovisita_id=\"b\".id "+
                            " INNER JOIN Estadio ON web_partido.estadio_id=Estadio.id " +
                            " WHERE EXTRACT(Year FROM web_partido.Fecha)="+anio)
            row = c.fetchall()

            return render_to_response('Web/partidoPorAnio.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True, 'Partido': row,
                                                               'Membresia': usuario.Membresia}))
        else:
            return render(request, 'Web/partidoPorAnio.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,})
    return redirect('/Web')

#--------------------------------------------------equipo vs equipo-------------------------------------------
def EquipovEquipo(request):
    if 'user' in request.session:
        usuario = Usuario.objects.get(Usuario=request.session['user'])
        equipos=Equipo.objects.all()
        if request.method == 'POST':
            equipo1=request.POST['equipo1']
            equipo2 = request.POST['equipo2']

            c = connection.cursor()
            c.execute(
                "SELECT web_partido.Fecha,web_partido.Resultado,\"a\".Nombre,\"b\".Nombre,Estadio.Nombre FROM web_partido "+
                " INNER JOIN Equipo \"a\" ON web_partido.equipolocal_id=\"a\".id "+
                " INNER JOIN Equipo \"b\" ON web_partido.equipovisita_id=\"b\".id "+
                " INNER JOIN Estadio ON web_partido.estadio_id=Estadio.id "+
                " WHERE (\"a\".Nombre='"+equipo1+"' and \"b\".Nombre='"+equipo2+"')"+
                " or (\"a\".Nombre='"+equipo2+"' and \"b\".Nombre='"+equipo1+"')" )
            row = c.fetchall()

            return render_to_response('Web/EquipovEquipo.html',
                                      RequestContext(request, {'var': request.session['tipo'],
                                                               'Usuario': usuario, 'buscando': True,
                                                               'Membresia': usuario.Membresia,
                                                               'Equipo':equipos,'Partido':row}))
        else:
            return render(request, 'Web/EquipovEquipo.html',
                          {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                           'Equipo':equipos})
    return redirect('/Web')

#--------------------------------------------------pagina para usuarios suscritos a ciertos equipos----------
def usuariosSuscritos(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipos = Equipo.objects.all()
            if request.method == 'POST':
                nombre = request.POST['equipo']
                equipo = Equipo.objects.get(id=nombre)
                seguir=Seguir.objects.filter(Equipo=equipo)
                return render_to_response('Web/usuariosSuscritos.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True, 'Equipo': equipos,
                                                                   'Membresia': usuario.Membresia, 'Seguir': seguir,'EquipoId':nombre}))
            else:
                return render(request, 'Web/usuariosSuscritos.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                               'Equipo': equipos})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#------------------------------------------------------CREAR PDF usuarios suscritos-------------------------------------------------
def usuariosSuscritosPDF(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipos = Equipo.objects.all()
            if request.method == 'POST':
                id = request.POST['pdfId']
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=UsuariosSuscritos.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)
                c.drawString(30,750,'Usuarios Suscritos')
                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Nombre''',styleBH)
                apellidoU=Paragraph('''Apellido''',styleBH)
                usuarioU=Paragraph('''Usuario''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                equipo=Equipo.objects.get(id=id)
                seguir=Seguir.objects.filter(Equipo=equipo)

                for seguir in seguir:
                    this_usuario=[seguir.Usuario.Nombre,seguir.Usuario.Apellido,seguir.Usuario.Usuario]
                    print(this_usuario)
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[1.9*cm,9.5*cm,1.9*cm,1.9*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/usuariosSuscritos.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                               'Equipo': equipos, 'buscando': True})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#----------------------------------------------------pagina usuarios con membresia-----------------------------------------------
def usuariosMembresia(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                tipo = request.POST['tipo']
                user=Usuario.objects.filter(Membresia=tipo,Tipo=1)
                return render_to_response('Web/usuariosMembresia.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True,
                                                                   'Membresia': usuario.Membresia, 'Usuario':user
                                                                    ,'TipoID':tipo}))
            else:
                return render(request, 'Web/usuariosMembresia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------------crear pdf usuarios membresia-------------------------------------------------
def usuariosMembresiaPDF(request):

    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                id = request.POST['pdfId']
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=UsuariosMembresia.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)
                if id=="1":
                    c.drawString(30,750,'Usuarios con Membresia')
                else:
                    c.drawString(30, 750, 'Usuarios sin Membresia')
                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Nombre''',styleBH)
                apellidoU=Paragraph('''Apellido''',styleBH)
                usuarioU=Paragraph('''Usuario''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                user=Usuario.objects.filter(Tipo=1,Membresia=id)

                for user in user:
                    this_usuario=[user.Nombre,user.Apellido,user.Usuario]
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[1.9*cm,9.5*cm,1.9*cm,1.9*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/usuariosMembresia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------- pagina con mas membresias---------------------------------------------------
def usuariosmasMembresia(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            c = connection.cursor()
            c.execute("SELECT * FROM( "+
                        " SELECT COUNT(Usuario.Usuario),Usuario.Nombre,Usuario.Usuario FROM Membresia "+
                        " INNER JOIN Usuario ON Membresia.usuario_id=Usuario.id "+
                        " GROUP BY Usuario.Nombre,Usuario.Usuario "+
                        " ORDER BY COUNT(Usuario.Usuario) DESC "+
                        " )A WHERE rownum<=5 ")
            row = c.fetchall()
            return render_to_response('Web/usuariosmasMembresia.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True,
                                                                   'Membresia': usuario.Membresia, 'Usuario':row}))
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------- crear pdf de usuarios con mas membresias------------------------------------
def usuariosmasMembresiaPDF(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=UsuariosmasMembresia.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)
                c.drawString(30,750,'Usuarios que han adquirio mas Membresias')

                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Nombre''',styleBH)
                apellidoU=Paragraph('''Usuario''',styleBH)
                usuarioU=Paragraph('''Cantidad''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                cc = connection.cursor()
                cc.execute("SELECT * FROM( " +
                          " SELECT COUNT(Usuario.Usuario),Usuario.Nombre,Usuario.Usuario FROM Membresia " +
                          " INNER JOIN Usuario ON Membresia.usuario_id=Usuario.id " +
                          " GROUP BY Usuario.Nombre,Usuario.Usuario " +
                          " ORDER BY COUNT(Usuario.Usuario) DESC " +
                          " )A WHERE rownum<=5 ")
                row = cc.fetchall()

                for user in row:
                    this_usuario=[user[1],user[2],user[0]]
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[1.9*cm,9.5*cm,1.9*cm,1.9*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/usuariosmasMembresia.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------pagina usuarios genero------------------------------------------------
def usuariosGenero(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                tipo = request.POST['tipo']
                user=Usuario.objects.filter(Genero=tipo,Tipo=1)
                return render_to_response('Web/usuariosGenero.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True,
                                                                   'Membresia': usuario.Membresia, 'Usuario':user
                                                                    ,'TipoID':tipo}))
            else:
                return render(request, 'Web/usuariosGenero.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------------crear pdf usuarios genero--------------------------------------------
def usuariosGeneroPDF(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                id = request.POST['pdfId']
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=UsuariosGenero.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)
                if id == "M":
                    c.drawString(30,750,'Usuarios Masculino')
                else:
                    c.drawString(30, 750, 'Usuarios Femeninos')
                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Nombre''',styleBH)
                apellidoU=Paragraph('''Apellido''',styleBH)
                usuarioU=Paragraph('''Usuario''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                user=Usuario.objects.filter(Tipo=1,Genero=id)

                for user in user:
                    this_usuario=[user.Nombre,user.Apellido,user.Usuario]
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[1.9*cm,9.5*cm,1.9*cm,1.9*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/usuariosGenero.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-----------------------------------------------------pagina para la edad----------------------------------------------

def usuarioEdad(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                edad = request.POST['edad']
                c = connection.cursor()
                c.execute("SELECT Nombre,Apellido,Usuario,2018-EXTRACT(Year FROM fecha_nac) FROM Usuario WHERE 2018-EXTRACT(Year FROM fecha_nac)>="+
                          edad + " and Tipo=1 ORDER BY Fecha_nac DESC")
                row = c.fetchall()
                return render_to_response('Web/usuariosEdad.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True,
                                                                   'Membresia': usuario.Membresia, 'Usuario':row
                                                                    ,'TipoID':edad}))
            else:
                return render(request, 'Web/usuariosEdad.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------------- crear edad pdf------------------------------------------------------
def usuarioEdadPDF(request):

    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                id = request.POST['pdfId']
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=UsuariosGenero.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)
                c.drawString(30, 750, 'Usuarios Edad')
                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Nombre''',styleBH)
                apellidoU=Paragraph('''Apellido''',styleBH)
                usuarioU=Paragraph('''Usuario''',styleBH)
                EdadU=Paragraph('''Edad''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU,EdadU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                cc = connection.cursor()
                cc.execute(
                    "SELECT Nombre,Apellido,Usuario,2018-EXTRACT(Year FROM fecha_nac) FROM Usuario WHERE 2018-EXTRACT(Year FROM fecha_nac)>=" +
                    id + " and Tipo=1 ORDER BY Fecha_nac DESC")
                row = cc.fetchall()

                for user in row:
                    this_usuario=[user[0],user[1],user[2],user[3]]
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[1.9*cm,9.5*cm,1.9*cm,1.9*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/usuarioEdad.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------- pagina empleados mas o menos----------------------------------------
def empleadosMASOMENOS(request):

    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                tipo = request.POST['tipo']
                c = connection.cursor()
                if tipo=="1":
                    c.execute(
                        "SELECT * FROM( "+
                            " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias "+
                            " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id "+
                            " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  "+
                            " ORDER BY Count(Noticias.Empleado_id) DESC "+
                            " )A WHERE rownum<=5")
                else:
                    c.execute(
                        "SELECT * FROM( " +
                        " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias " +
                        " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id " +
                        " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  " +
                        " ORDER BY Count(Noticias.Empleado_id) ASC " +
                        " )A WHERE rownum<=5")
                row = c.fetchall()
                return render_to_response('Web/empleadosMASOMENOS.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True,
                                                                   'Membresia': usuario.Membresia, 'Usuario':row
                                                                    ,'TipoID':tipo}))
            else:
                return render(request, 'Web/empleadosMASOMENOS.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------- PDF EMPLEADOS MAS O MENOS-----------------------------------------------
def empleadosMASOMENOSPDF(request):

    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            if request.method == 'POST':
                id = request.POST['pdfId']
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=UsuariosMasOMenos.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)
                cc = connection.cursor()

                if id == "1":
                    c.drawString(30,750,'Empleados que mas han publicado')
                    cc.execute(
                        "SELECT * FROM( " +
                        " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias " +
                        " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id " +
                        " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  " +
                        " ORDER BY Count(Noticias.Empleado_id) DESC " +
                        " )A WHERE rownum<=5")
                else:
                    c.drawString(30, 750, 'Empleados que menos han publicado')
                    cc.execute(
                        "SELECT * FROM( " +
                        " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias " +
                        " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id " +
                        " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  " +
                        " ORDER BY Count(Noticias.Empleado_id) ASC " +
                        " )A WHERE rownum<=5")
                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Nombre''',styleBH)
                apellidoU=Paragraph('''Apellido''',styleBH)
                usuarioU=Paragraph('''Usuario''',styleBH)
                cantidadU=Paragraph('''Cantidad''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU,cantidadU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                row = cc.fetchall()

                for user in row:
                    this_usuario=[user[1],user[2],user[3],user[0]]
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[4*cm,4*cm,4*cm,1.9*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/empleadosMASOMENOS.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#--------------------------------------------------- pagina empleados mas o menos equipo----------------------------------------
def empleadosMASOMENOSEquipo(request):

    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipos=Equipo.objects.all()
            if request.method == 'POST':
                tipo = request.POST['tipo']
                equipo=request.POST['equipo']
                c = connection.cursor()
                if tipo=="1":
                    c.execute(
                        "SELECT * FROM( "+
                            " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias "+
                            " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id "+
                            " INNER JOIN Equipo ON Noticias.equipo_id=Equipo.id "+
                            " WHERE Equipo.Nombre='"+equipo+"' "+
                            " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  "+
                            " ORDER BY Count(Noticias.Empleado_id) DESC "+
                            " )A WHERE rownum<=5")
                else:
                    c.execute(
                        "SELECT * FROM( " +
                        " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias " +
                        " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id " +
                        " INNER JOIN Equipo ON Noticias.equipo_id=Equipo.id " +
                        " WHERE Equipo.Nombre='" + equipo + "' " +
                        " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  " +
                        " ORDER BY Count(Noticias.Empleado_id) ASC " +
                        " )A WHERE rownum<=5")
                row = c.fetchall()
                return render_to_response('Web/empleadosMASOMENOSEquipo.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True,
                                                                   'Membresia': usuario.Membresia, 'Usuario':row
                                                                    ,'TipoID':tipo,'EquipoNombre':equipo,'Equipo':equipos}))
            else:
                return render(request, 'Web/empleadosMASOMENOSEquipo.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,'Equipo':equipos})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------- pdf empleados mas o menos-------------------------------------------------------
def empleadosMASOMENOSEquipoPDF(request):


    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipos=Equipo.objects.all()

            if request.method == 'POST':
                id = request.POST['pdfId']
                equipo=request.POST['pdfId2']
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=UsuariosMasOMenos.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)
                cc = connection.cursor()

                if id == "1":
                    c.drawString(30,750,'Empleados que mas han publicado: '+equipo)
                    cc.execute(
                        "SELECT * FROM( " +
                        " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias " +
                        " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id " +
                        " INNER JOIN Equipo ON Noticias.equipo_id=Equipo.id " +
                        " WHERE Equipo.Nombre='" + equipo + "' " +
                        " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  " +
                        " ORDER BY Count(Noticias.Empleado_id) DESC " +
                        " )A WHERE rownum<=5")
                else:
                    c.drawString(30, 750, 'Empleados que menos han publicado: '+equipo)
                    cc.execute(
                        "SELECT * FROM( " +
                        " SELECT Count(Noticias.Empleado_id),Usuario.Nombre,Usuario.Apellido,Usuario.Usuario FROM Noticias " +
                        " INNER JOIN Usuario ON Noticias.Empleado_id=Usuario.id " +
                        " INNER JOIN Equipo ON Noticias.equipo_id=Equipo.id " +
                        " WHERE Equipo.Nombre='" + equipo + "' " +
                        " GROUP BY Empleado_id,Usuario.Nombre,Usuario.Apellido,Usuario.Usuario  " +
                        " ORDER BY Count(Noticias.Empleado_id) ASC " +
                        " )A WHERE rownum<=5")
                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Nombre''',styleBH)
                apellidoU=Paragraph('''Apellido''',styleBH)
                usuarioU=Paragraph('''Usuario''',styleBH)
                cantidadU=Paragraph('''Cantidad''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU,cantidadU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                row = cc.fetchall()

                for user in row:
                    this_usuario=[user[1],user[2],user[3],user[0]]
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[4*cm,4*cm,4*cm,1.9*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/empleadosMASOMENOSEquipo.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                               'Equipo': equipos})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------- pagina bitacora admins---------------------------------------------------------
def bitacoraAdmins(request):
    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            bitacora=Historial.objects.all()
            return render_to_response('Web/bitacoraAdmins.html',
                                          RequestContext(request, {'var': request.session['tipo'],
                                                                   'Usuario': usuario, 'buscando': True,
                                                                   'Membresia': usuario.Membresia, 'Bitacora':bitacora}))
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#-------------------------------------------- pdf de bitacora de admins-------------------------------------------------------
def bitacoraAdminsPDF(request):


    if 'user' in request.session:
        if request.session['tipo']==1:
            usuario = Usuario.objects.get(Usuario=request.session['user'])
            equipos=Equipo.objects.all()

            if request.method == 'POST':
                response=HttpResponse(content_type='application/pdf')
                response['Content-Disposition']='attachment; filename=Bitacora.pdf'
                buffer=BytesIO()
                c=canvas.Canvas(buffer,pagesize=A4)

                c.setLineWidth(.3)
                c.setFont('Helvetica',22)


                c.drawString(30, 750, 'BITACORA')
                c.setFont('Helvetica', 12)
                c.drawString(30, 735, 'REPORTE')

                styles=getSampleStyleSheet()
                styleBH=styles["Normal"]
                styleBH.aligment=TA_CENTER
                styleBH.fontSize=10

                nombreU=Paragraph('''Admin''',styleBH)
                apellidoU=Paragraph('''Usuario Afectado''',styleBH)
                usuarioU=Paragraph('''Accion''',styleBH)
                cantidadU=Paragraph('''Descripcion''',styleBH)

                data=[]
                data.append([nombreU,apellidoU,usuarioU,cantidadU])

                styles=getSampleStyleSheet()
                styleN=styles["BodyText"]
                styleN.aligment=TA_CENTER
                styleN.fontSize=7

                width,height=A4
                high=650

                bitacora=Historial.objects.all()
                for user in bitacora:
                    this_usuario=[user.Admin.Usuario,user.Usuario.Usuario,user.Accion,user.Descripcion]
                    data.append(this_usuario)
                    high=high-18

                table=Table(data,colWidths=[4*cm,4*cm,4*cm,8*cm])
                table.setStyle(TableStyle([
                    ('INNERGRID',(0,0),(-1,-1),0.25,colors.black),
                    ('BOX',(0,0),(-1,-1),0.25,colors.black),
                ]))

                table.wrapOn(c,width,height)
                table.drawOn(c,30,high)
                c.showPage()


                c.save()
                pdf=buffer.getvalue()
                buffer.close()
                response.write(pdf)
                return response

            else:
                return render(request, 'Web/bitacoraAdmins.html',
                              {'Usuario': usuario, 'var': request.session['tipo'], 'Membresia': usuario.Membresia,
                               'Equipo': equipos})
        return render(request, 'Web/error.html',{})
    return redirect('/Web')

#---------------------------------------------pagina de error-----------------------------------------------------------------
def errorPage(request):
    return redirect('/Web/error.html')


#---------------------------------------------------------ANDROID------------------------------------------------------------
#------------------------------------------------------------------------------------------------------------------------------

#---------------------------------------------------------LOGIN---------------------------------------------
class UsuarioList(generics.ListAPIView):
    serializer_class=UsuarioSerializer

    def get_queryset(self):
        query_params=self.request.query_params
        pk = query_params.get('pk',None)
        contra = query_params.get('contra', None)
        query=Usuario.objects.all()
        query=query.filter(Usuario=pk,Contrasenia=contra,Estado=1)
        if query.exists():
            return query
        raise Http404

#-------------------------------------------------------- CREAR USUARIOS-------------------------------------------
@api_view(['GET','POST'])
def UsuarioPost(request):
    if request.method=='POST':
        nombre = request.POST['nombre']
        usuario=request.POST['usuario']
        apellido = request.POST['apellido']
        correo = request.POST['correo']
        telefono = request.POST['numero']
        genero = request.POST['genero']
        direccion = request.POST['direccion']
        fecha_nac = request.POST['fecha']
        contra=request.POST['contrasenia']
        date = datetime.datetime.strptime(fecha_nac, '%d/%m/%Y').date()
        user=Usuario.objects.filter(Usuario=usuario)
        if not user.exists():
            user = Usuario(Nombre=nombre, Usuario=usuario, Contrasenia=contra, Tipo=1, Apellido=apellido, Correo=correo,
                                 Telefono=telefono, Imagen='./Default.png', Genero=genero, Direccion=direccion, Fecha_nac=date)
            user.save()

            salt = hashlib.sha1(str(random.random())).hexdigest()[:5]
            activation_key = hashlib.sha1(salt + correo).hexdigest()
            key_expires = datetime.datetime.today() + datetime.timedelta(2)

            new_profile = UserProfile(user=user, activation_key=activation_key, key_expires=key_expires)
            new_profile.save()

            email_subject = "Verificar Cuenta"
            email_body = "Hola %s Gracias pro registrarte. Para activar tu cuenta da click en este link en menos de 48 horas: http://127.0.0.1:8000/Web/confirm/%s" % (
                usuario, activation_key)

            send_mail(email_subject, email_body, 'carloshernandez.chsegunda@gmail.com',
                      [correo], fail_silently=False)
            return Response({"Error": "0", "data": request.data})

    return Response({"Error": "1","data":"Ya existe ese usuario"})


#-------------------------------------------------------- OBTENER TODAS LAS NOTICIAS-------------------------------------------
@api_view(['GET'])
def getAllNoticiasA(request):
    if request.method=='GET':

       cc = connection.cursor()
       cc.execute(" SELECT Noticias.Titulo,Noticias.Cuerpo,to_char(Noticias.Fecha,\'DD.MM.YYYY\'),Equipo.Nombre,Usuario.Nombre,Usuario.Apellido,Noticias.foto FROM NOTICIAS "
                    " INNER JOIN Equipo ON Noticias.equipo_id=Equipo.id "+
                    " INNER JOIN Usuario ON Noticias.empleado_id=Usuario.id "+
                    " ORDER BY Noticias.Fecha DESC ")
       row = cc.fetchall()
       result=[]
       keys=('titulo','cuerpo','fecha','equipo','nombre','apellido','imagen')
       for rows in row:
           result.append(dict(zip(keys,rows)))
       json_data=json.dumps(result)
       return HttpResponse(json_data,content_type="application/json")

    raise Http404

#-------------------------------------------------------- OBTENER TODAS LAS NOTICIAS-------------------------------------------
@api_view(['POST'])
def NoticiasFiltradasA(request):
    if request.method=='POST':
       usuario=request.POST['usuario']
       cc = connection.cursor()
       cc.execute("SELECT Noticias.Titulo,Noticias.Cuerpo,to_char(Noticias.Fecha,\'DD.MM.YYYY\'),Equipo.Nombre,\"t\".Nombre,\"t\".Apellido,Noticias.foto FROM Usuario "+
                    " INNER JOIN Seguir ON Usuario.id=Seguir.usuario_id "+
                    " INNER JOIN Noticias ON Seguir.equipo_id=Noticias.equipo_id "+
                    " INNER JOIN Equipo ON Noticias.equipo_id=Equipo.id "+
                    " INNER JOIN Usuario \"t\" ON Noticias.empleado_id=\"t\".id "+
                    " WHERE Usuario.Usuario=\'"+usuario+"\' "
                    " ORDER BY Noticias.Fecha DESC ")
       row = cc.fetchall()
       result=[]
       keys=('titulo','cuerpo','fecha','equipo','nombre','apellido','imagen')
       for rows in row:
           result.append(dict(zip(keys,rows)))
       json_data=json.dumps(result)
       return HttpResponse(json_data,content_type="application/json")

    raise Http404

#--------------------------------------------------------------Cancelar Suscripcion-----------------------------------------------
@api_view(['POST'])
def cancelarSuscripcionA(request):
    if request.method=='POST':
        usuario = request.POST['usuario']
        user=Usuario.objects.filter(Usuario=usuario)
        user.update(Membresia=0)
        return Response({"Error": "0", "data": "Cancelada con exito"})
    raise Http404

#----------------------------------------------------------comprar membresia-------------------------------------------------
@api_view(['POST'])
def comprarMembresiaA(request):
    if request.method=='POST':
        usuario = Usuario.objects.filter(Usuario=request.POST['usuario'])
        usuario.update(Membresia=1)
        usuario = Usuario.objects.get(Usuario=request.POST['usuario'])
        fecha_fin = datetime.datetime.today() + datetime.timedelta(30)
        membresia = Membre(Fecha_fin=fecha_fin, Usuario=usuario)
        membresia.save()
        return Response({"Error": "0", "data": "Comprada con exito"})
    raise Http404

#----------------------------------------------------------- consultas partidos-------------------------------------------------
@api_view(['POST'])
def buscarPartidosA(request):
    if request.method=='POST':
        tipo = request.POST['buscar']
        usuario=request.POST['usuario']
        cc=connection.cursor()
        if tipo == 'Todos':
            cc.execute("SELECT to_char(web_partido.fecha,'DD.MM.YY'),web_partido.resultado,web_partido.asistencia,web_partido.estado, "+
                " Competencia.Nombre,\"a\".Nombre,\"b\".Nombre,Estadio.Nombre FROM web_partido "+
                " INNER JOIN Competencia ON web_partido.competencia_id=Competencia.id "+
                " INNER JOIN Equipo \"a\" ON web_partido.equipoLocal_id=\"a\".id "
                " INNER JOIN Equipo \"b\" ON web_partido.equipoVisita_id=\"b\".id "
                " INNER JOIN Estadio ON web_partido.estadio_id=Estadio.id ORDER BY web_partido.fecha DESC")
        elif tipo == 'Favoritos':
            cc.execute("SELECT to_char(web_partido.fecha,'DD.MM.YY'),web_partido.resultado,web_partido.asistencia,web_partido.estado, "+
                " Competencia.Nombre,\"a\".Nombre,\"b\".Nombre,Estadio.Nombre From Seguir "+
                " INNER JOIN Usuario ON Seguir.usuario_id=Usuario.id "+
                " INNER JOIN web_partido ON Seguir.equipo_id=web_partido.equipoLocal_id or Seguir.equipo_id=web_partido.equipovisita_id "+
                " INNER JOIN Competencia ON web_partido.competencia_id=Competencia.id "+
                " INNER JOIN Equipo \"a\" ON web_partido.equipoLocal_id=\"a\".id "+
                " INNER JOIN Equipo \"b\" ON web_partido.equipoVisita_id=\"b\".id "+
                " INNER JOIN Estadio ON web_partido.estadio_id=Estadio.id "
                " WHERE Usuario.Usuario='"+usuario+"' "+
                " ORDER BY web_partido.fecha DESC")
        else:
            cc.execute(
                "SELECT to_char(web_partido.fecha,'DD.MM.YY'),web_partido.resultado,web_partido.asistencia,web_partido.estado, " +
                " Competencia.Nombre,\"a\".Nombre,\"b\".Nombre,Estadio.Nombre FROM web_partido " +
                " INNER JOIN Competencia ON web_partido.competencia_id=Competencia.id " +
                " INNER JOIN Equipo \"a\" ON web_partido.equipoLocal_id=\"a\".id "
                " INNER JOIN Equipo \"b\" ON web_partido.equipoVisita_id=\"b\".id "
                " INNER JOIN Estadio ON web_partido.estadio_id=Estadio.id "+
                " WHERE web_partido.estado='"+tipo+"' ORDER BY web_partido.fecha DESC")

        row = cc.fetchall()
        result = []
        keys = ('fecha', 'resultado', 'asistencia', 'estado', 'competencia', 'equipoLocal', 'equipoVisita','estadio')
        for rows in row:
            result.append(dict(zip(keys, rows)))
        json_data = json.dumps(result)
        return HttpResponse(json_data, content_type="application/json")
    raise Http404

#-------------------------------------------------- obtener todos los equipos----------------------------------------------------------------------
@api_view(['GET'])
def obtenerEquiposA(request):
    if request.method=='GET':
        query=Equipo.objects.all()
        serializer=EquiposSerialize(query,many=True)
        return Response(serializer.data)
    raise  Http404

#------------------------------------------------- jugadores tecnicos por equipo-------------------------------------
@api_view(['POST'])
def JugadorDTA(request):
    if request.method=='POST':
        equipo=request.POST['equipo'];
        cc=connection.cursor()
        cc.execute("SELECT to_char(web_bitacorajugador.fecha_inicio,\'DD.MM.YY\'),to_char(web_bitacorajugador.fecha_fin,\'DD.MM.YY\'), "+
            " Jugador.Nombre,0 " +
            " FROM web_BitacoraJugador "+
            " INNER JOIN Jugador ON web_bitacorajugador.jugador_id=Jugador.id "+
            " INNER JOIN Equipo ON web_bitacorajugador.equipo_id=Equipo.id "+
            " WHERE Equipo.Nombre='"+equipo+"' "+
            " union "+
            " SELECT to_char(bitacora_dt.fecha_inicio,\'DD.MM.YY\'),to_char(bitacora_dt.fecha_fin,\'DD.MM.YY\'), "+
            " DT.Nombre,1 "+
            " FROM bitacora_dt "+
            " INNER JOIN DT ON bitacora_dt.dt_id=DT.id "+
            " INNER JOIN Equipo ON bitacora_dt.equipo_id=Equipo.id "+
            " WHERE Equipo.Nombre='"+equipo+"' ")


        row = cc.fetchall()
        result = []
        keys = ('fechainicio', 'fechafin', 'nombre', 'tipo')
        for rows in row:
            result.append(dict(zip(keys, rows)))
        json_data = json.dumps(result)
        return HttpResponse(json_data, content_type="application/json")
    raise Http404


#---------------------------------------------- jugadores tecnicos por edad--------------------------------------------------
@api_view(['POST'])
def JugadorDEA(request):
    if request.method=='POST':
        edad=request.POST['edad'];
        cc=connection.cursor()
        cc.execute("SELECT Nombre ,2018-to_char(fecha_nac,\' YYYY \'),0 FROM Jugador "+
            " WHERE (2018-to_char(fecha_nac,\'YYYY\'))>"+edad+
           "  union "+
           "  SELECT Nombre ,2018-to_char(fecha_nac,\'YYYY\'),1 FROM DT "+
            " WHERE (2018-to_char(fecha_nac,\'YYYY\'))>"+edad)


        row = cc.fetchall()
        result = []
        keys = ('nombre', 'edad', 'tipo')
        for rows in row:
            result.append(dict(zip(keys, rows)))
        json_data = json.dumps(result)
        return HttpResponse(json_data, content_type="application/json")
    raise Http404

#----------------------------------------------- jugadores tecnicos por edad menor------------------------------------------------------
@api_view(['POST'])
def JugadorDEAme(request):
    if request.method == 'POST':
        edad = request.POST['edad'];
        cc = connection.cursor()
        cc.execute("SELECT Nombre ,2018-to_char(fecha_nac,\' YYYY \'),0 FROM Jugador " +
                   " WHERE (2018-to_char(fecha_nac,\'YYYY\'))<" + edad +
                   "  union " +
                   "  SELECT Nombre ,2018-to_char(fecha_nac,\'YYYY\'),1 FROM DT " +
                   " WHERE (2018-to_char(fecha_nac,\'YYYY\')) <" + edad)

        row = cc.fetchall()
        result = []
        keys = ('nombre', 'edad', 'tipo')
        for rows in row:
            result.append(dict(zip(keys, rows)))
        json_data = json.dumps(result)
        return HttpResponse(json_data, content_type="application/json")
    raise Http404

