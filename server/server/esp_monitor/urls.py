from django.urls import path
from . import views
urlpatterns=[
    path("",views.espmon,name='espmon'),
    path("api",views.data,name="api")
]