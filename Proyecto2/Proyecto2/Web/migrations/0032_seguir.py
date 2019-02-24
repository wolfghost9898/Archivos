# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0031_auto_20181030_1839'),
    ]

    operations = [
        migrations.CreateModel(
            name='Seguir',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Equipo', models.ForeignKey(related_name='EquipoSeguir', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('Usuario', models.ForeignKey(related_name='UsuarioSeguir', on_delete=django.db.models.deletion.PROTECT, to='Web.Usuario')),
            ],
            options={
                'db_table': 'Seguir',
            },
        ),
    ]
