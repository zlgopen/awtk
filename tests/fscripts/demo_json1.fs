//load from string
a=json_load('
  {
    "tom": {
      "name":"tom", 
      "age":100
    },
    "jim": {
      "name":"jim", 
      "age":200
    },
    "kite": {
      "name" : {
        "first": "kite",
        "last" : "gates"
      }
    }
  }
')

print(object_get(a, "tom.age"));
print(object_get(a, "jim.age"));
print(object_get(a, "kite.name.first"));
print(object_get(a, "kite.name.last"));
print(object_get(a, "kite"));

