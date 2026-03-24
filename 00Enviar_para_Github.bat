@echo off
echo Iniciando sincronizacao com o GitHub...
d:
cd "D:\Scripts personalizados"

:: Adiciona todas as alterações
git add .

:: Cria o commit com a data e hora atual
git commit -m "Sincronizacao automatica: %date% %time%"

:: Envia para o GitHub
git push origin main

echo.
echo Concluido! Seus scripts estao no GitHub.
pause