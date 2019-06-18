<?php $inu = new Par3_Controller_InstrumentoUnidade();
$itrid = $inu->pegarItrid($_GET['inuid']);
$inunome = $inu->pegarNomeEntidade($_GET['inuid']);


$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();
if ($itrid === '2') {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;

} else {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO;
}

$objPessoaFisica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($_GET['inuid'], $tenid);
?>


<p align="center">
    <strong>TERMO DE ADESÃO AO NOVO CICLO DO PROGRAMA PACTO NACIONAL PELA ALFABETIZAÇÃO NA IDADE CERTA</strong>
</p>
<p>
    <br/>
    O Estado de <?php echo $inunome?>, representado pelo(a) Secretário(a) Estadual de Educação, <?php echo $objPessoaFisica->entnome?>, resolve firmar o presente Termo de
    Adesão ao novo ciclo do Programa Pacto Nacional pela Alfabetização na Idade Certa, consideradas as seguintes condições:
</p>
<p>
    <strong>CLÁUSULA PRIMEIRA - DO OBJETO</strong>
</p>
<p>
    O presente termo tem por objeto a adesão do Estado ao novo ciclo do Programa Pacto Nacional pela Alfabetização na Idade Certa, que reafirma e amplia, em
    consonância com a meta 5 do Plano Nacional de Educação - PNE e com o Decreto nº 6.094, de 24 de abril de 2007, o compromisso de alfabetizar as crianças
    até, no máximo, os oito anos de idade, ao final do 3º ano do ensino fundamental.
</p>
<p>
    <strong>CLÁUSULA SEGUNDA - DAS RESPONSABILIDADES DO MINISTÉRIO DA EDUCAÇÃO</strong>
</p>
<p>
    O Ministério da Educação, em regime de colaboração com os entes federados, reafirma seu comprometimento com a alfabetização e o letramento de todas as
    crianças na idade adequada. Para tal, assume a responsabilidade por: promover atividades de formação continuada de coordenadores, orientadores de estudo e
    professores atuantes no ciclo de alfabetização, com aproximação entre as ações de formação e a prática docente; fortalecer e responsabilizar as estruturas
    estaduais de gestão regional da educação básica, intensificando a cooperação entre estados e municípios na coordenação das ações do programa; envolver as
    gestões estaduais e municipais no monitoramento das ações do programa e na avaliação periódica dos resultados dos estudantes; apoiar o desenvolvimento e a
    impressão de material de formação e apoio à prática docente, com foco na aprendizagem do estudante, por iniciativa de estados e em parceria com
    instituições públicas de ensino superior; promover o reconhecimento e a valorização das escolas e profissionais comprometidos com a evolução na
    alfabetização dos alunos das redes de ensino; e envidar esforços para apoiar as escolas com maior dificuldade na alfabetização e letramento de alunos no
    ensino fundamental. Tais responsabilidades se consolidam a partir do compromisso com as seguintes ações, dentre outras:
</p>
<p>
    I - Fomentar ações de mobilização e de gestão do Programa, com a indução da criação de instâncias regionais de gestão compartilhada (estado e municípios),
    responsáveis pelo monitoramento das ações do programa e pela avaliação periódica dos resultados dos estudantes;
</p>
<p>
    II - Promover, em parceria com os sistemas públicos de ensino dos Estados, Distrito Federal e Municípios e as instituições de ensino superior, a formação
    dos coordenadores das ações do Programa nos estados, no Distrito Federal e nos municípios, dos orientadores de estudo e dos professores cursistas nas redes
    de ensino que aderirem às ações do Programa, inclusive com o fomento à formulação e difusão de material de apoio à formação docente, com foco na
    aprendizagem do aluno;
</p>
<p>
    III - Conceder bolsas de apoio para incentivar a participação dos orientadores de estudo, professores cursistas e coordenadores das Redes nas atividades de
    formação nas redes de ensino que aderirem às ações do Programa;
</p>
<p>
    IV - Realizar avaliações anuais universais, pelo Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira - INEP.
</p>
<p>
    <strong>CLÁUSULA TERCEIRA - DAS RESPONSABILIDADES DO ESTADO</strong>
</p>
<p>
    O Estado confirma sua adesão aos compromissos assumidos pela União, e reafirma seu comprometimento e responsabilização com a alfabetização e o letramento
    de todas as crianças até, no máximo, os oito anos de idade, ao final do 3º ano do ensino fundamental. Para tal, assume a responsabilidade por: assegurar a
    constituição de equipes especializadas nos temas da alfabetização e do letramento em suas unidades regionais de gestão da educação básica, intensificando a
    cooperação entre estados e municípios na coordenação das ações do programa; garantir a implementação dos cursos de formação continuada em suas redes;
    assegurar o monitoramento das ações do programa e a avaliação periódica dos resultados dos estudantes; promover o reconhecimento e a valorização das
    escolas e profissionais comprometidos com a evolução na alfabetização dos estudantes das redes de ensino; e envidar esforços para apoiar as escolas com
    maior dificuldade na alfabetização e letramento de estudantes no ensino fundamental. Tais responsabilidades se consolidam a partir do compromisso com as
    seguintes ações, dentre outras:
</p>
<p>
    I - Instituir, em parceria com a Undime, um Comitê Gestor em cada estado, com o intuito de viabilizar o desenvolvimento das ações do Programa, definindo
    coletivamente os membros integrantes e assegurando a participação das IES públicas responsáveis pela formação;
</p>
<p>
    II - Viabilizar e participar de instâncias regionais de gestão compartilhada (estado e municípios) responsáveis pelo monitoramento das ações do Programa e
    pela avaliação periódica dos resultados dos estudantes;
</p>
<p>
    III - Sugerir diretrizes pedagógicas vinculadas às políticas de formação continuada de professores com foco na alfabetização e letramento, em parceria com
    as universidades, ou consórcio de universidades e centros de formação, responsáveis pela formação no estado (rede municipal e estadual), bem como indicar
    os formadores da rede que ministrarão o curso de formação aos orientadores de estudo em parceria com as IES responsáveis pela formação no estado;
</p>
<p>
    IV - designar coordenadores para se dedicar às ações do Programa e alocar equipes necessárias para a sua gestão, inclusive em suas unidades regionais, além
    de custear o seu deslocamento e a sua hospedagem para os eventos de formação, bem como disponibilizar Assistência Técnica aos municípios com maiores
    dificuldades na implementação das ações do Programa e na melhoria da alfabetização dos alunos de 1º a 3º ano e escolas públicas do ensino fundamental;
</p>
<p>
    V - fomentar e garantir a participação dos orientadores de estudo e dos professores cursistas de sua rede de ensino nas atividades de formação, sem
    prejuízo da carga-horária em sala de aula, custeando o deslocamento e a hospedagem, sempre que necessário;
</p>
<p>
    VI - firmar o compromisso de buscar uma evolução na alfabetização dos estudantes das redes de ensino considerando a Avaliação Nacional de Alfabetização,
    bem como outros instrumentos adequados.
</p>
<p align="right">
    <?php echo $inunome?>, <?php echo date('d')?>/<?php echo date('m')?>/<?php echo date('Y')?>
</p>
<br>
<br>
<p align="center">
    <?php echo $objPessoaFisica->entnome?>
</p>
<p align="center">
    Secretário(a) Estadual de Educação de <?php echo $inunome?>
</p>