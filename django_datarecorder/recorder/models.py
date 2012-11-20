from django.db import models

# Create your models here.

class SensorModel(models.Model):
    recordingIsActive = models.BooleanField()
    sampleRateHz = models.FloatField()
    arithmeticMeanRateHz = models.FloatField();
    sensorSystemName = models.CharField(max_length=64, unique=True)
    sensorType = models.CharField(max_length=64)
    ringBufferSize = models.IntegerField()
    
class SensorEnvironmentModel(models.Model):
    sensor = models.ForeignKey(SensorModel)
    
class SensorDataValueModel(models.Model):
    sensor = models.ForeignKey(SensorModel)
    dateTime = models.DateTimeField()
    value = models.FloatField()
    
class EnvironmentModel(models.Model):
    globalRecordingIsActive = models.BooleanField()
    realTimeViolationCounter = models.IntegerField()