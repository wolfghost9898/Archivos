# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import Web.models


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Usuario',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Usuario', models.CharField(max_length=45)),
                ('Contrasenia', models.CharField(max_length=45)),
                ('Tipo', models.IntegerField(default=0)),
                ('Nombre', models.CharField(max_length=45)),
                ('Apellido', models.CharField(max_length=45)),
                ('Correo', models.CharField(max_length=70)),
                ('Telefono', models.CharField(max_length=10)),
                ('Imagen', models.ImageField(upload_to=Web.models.get_image_path, blank=True)),
                ('Genero', models.CharField(max_length=5)),
                ('Fecha_reg', models.DateTimeField(auto_now_add=True)),
                ('Direccion', models.TextField()),
                ('Fecha_nac', models.DateTimeField(auto_now_add=True)),
            ],
        ),
    ]
