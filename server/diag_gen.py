import matplotlib.pyplot as plt
import pandas as pd

# Charger les données depuis le fichier CSV
data = pd.read_csv('data.csv')
print(data)

# Extraire les colonnes nécessaires
num_capteur = data['Num_capteur']
temperature = data['Température']
hygrometrie = data['Hygrométrie']

# Créer le diagramme en barres groupées
barWidth = 0.35
r1 = range(len(num_capteur))
r2 = [x + barWidth for x in r1]

plt.bar(r1, temperature, color='blue', width=barWidth, edgecolor='grey', label='Température')
plt.bar(r2, hygrometrie, color='orange', width=barWidth, edgecolor='grey', label='Hygrométrie')

plt.xlabel('Numéro de Capteur')
plt.ylabel('Valeurs')
plt.xticks([r + barWidth/2 for r in range(len(num_capteur))], num_capteur)
plt.title('Diagramme de Température et Hygrométrie par Capteur')
plt.legend()
plt.savefig('diag.png')
