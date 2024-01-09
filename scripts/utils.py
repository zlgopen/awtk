def strtobool(value):
  if(isinstance(value, str)):
    value = value.lower()
    if value in ("y", "yes", "on", "1", "true", "t"):
      return True
  return False
