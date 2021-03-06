# Generated by Django 2.2.12 on 2020-08-29 11:00

from django.db import migrations, models
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0003_registerdevice'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='registerdevice',
            name='id',
        ),
        migrations.AddField(
            model_name='registerdevice',
            name='created_at',
            field=models.DateTimeField(auto_now_add=True, default=django.utils.timezone.now),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='registerdevice',
            name='updated_at',
            field=models.DateTimeField(auto_now=True),
        ),
        migrations.AlterField(
            model_name='registerdevice',
            name='uuid',
            field=models.CharField(max_length=36, primary_key=True, serialize=False),
        ),
    ]
