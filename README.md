# RattletrapEngine
SDL2 Game Engine  

----------------

Sistema de uso do repositório (em adaptação):  
- Branch dev saindo de master.  
- Um branch para cada feature saindo de dev. O branch será nomeado "feature/*" onde "*" será o nome da feature. Exemplo "feature/drag-n-drop".  
- Para juntar qualquer feature ao dev, será um pull request que deve ser revisado e testado por pelo menos um outro programador, ou seja, você nunca aceitará seus próprios pull requests. O branch será excluído assim que o pull request for aceito e realizado.  
- Se uma feature for complexa (por exemplo UI), vc pode fazer sub branches. Exemplo "feature/UI/animacao-telas". Todas as regras acima se aplicam.  
- Semanalmente, ou quinzenalmente, faríamos um merge do tweaks (descrito mais abaixo) pro master e depois do dev pro master, atualizando o número da versão minor. E depois um merge do master pro dev.  
- A documentação deve estar atualizada e refletindo com precisão o branch dev ANTES do merge ser feito.  
- Extremamente recomendável de que cada pull request de features já estejam com sua documentação correta.  
- Merges e pull requests só devem ser feitos se o código compilar e executar sem game-breaking bugs.  
- Branches de correção de bugs saem de master com nomenclatura "bugfix/*". Onde * é o número da issue no github. Exemplo "bugfix/32".  
- Branches de bugfix se juntam ao master da mesma forma que um de feature se junta ao dev. Mas atualizando a versão de patch.  
- Branches de adição ou grande alterações de arte/música serão em branches de feature, preferencialmente compostos de um único commit. Numeração de versão segue o mesmo padrão.  
- Tweaks será um branch saindo de master. Ele será feito de commits únicos que ajustam coisas. Tipo nome de arquivo de música, texto que aparece sei lá onde, etc.  
- O branch tweaks não receberá um merge do master toda vez, somente pela primeira vez que for ter um patch naquela versão. Cada commit altera o número da versão de patch.  
- A numeração de versão será semântico, ou seja, MAJOR.MINOR.PATCH.TWEAK . Qualquer atualização em um número zera todos os números à sua direita.  
- Commits DEVEM ter nomes (e, se aplicáveis, resumos) descritivos. Nada de "asdfgh" ou "teste", etc...  
- Se possível, colocar um resumo de tudo que foi alterado num pull request.  
- Entregáveis receberão etiquetas (tags) com o valor 30, 70 ou 100.  

