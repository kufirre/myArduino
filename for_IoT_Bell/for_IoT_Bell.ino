 boolean arrayIncludeElement(int array[], int element);
 boolean arrayIncludeElement(int array[], int element) {
 for (int i = 0; i < max; i++) {
      if (array[i] == element) {
          return true;
      }
    }
  return false;
 }
