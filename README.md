	Данное приложение способено решить обычную и геометрическую судоку любого размера.
	Для решения используется метод синглов и перебор с возвратом. Если метод синглов не находит решение, то
подключается перебор. Существенную экономию ресурсов при переборе вносит, тот факт, что в пустую клетку 
подставляется не любое возможное значени, а только кондидат, который формируются взависимости от значений других
клеток в группе, столбце и строке.