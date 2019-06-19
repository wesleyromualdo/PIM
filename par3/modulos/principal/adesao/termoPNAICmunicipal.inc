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
    <strong>TERMO DE COMPROMISSO AO NOVO CICLO DO PROGRAMA PACTO NACIONAL PELA ALFABETIZAÇÃO NA IDADE CERTA</strong>
</p>
<p>
    <br/>
    O Município de <?php echo $inunome?>, representado pelo(a) Secretário(a) Municipal de Educação, <?php echo $objPessoaFisica->entnome?>),resolve firmar o presente
    Termo de Adesão ao novo ciclo do Programa Pacto Nacional pela Alfabetização na Idade Certa, consideradas as seguintes condições:
</p>
<p>
    <strong>CLÁUSULA PRIMEIRA - DO OBJETO</strong>
</p>
<p>
    O presente termo tem por objeto a adesão do Município ao novo ciclo do Programa Pacto Nacional pela Alfabetização na Idade Certa, que reafirma e amplia,
    em consonância com a meta 5 do Plano Nacional de Educação - PNE e com o Decreto nº 6.094, de 24 de abril de 2007, o compromisso de alfabetizar as crianças até,
    no máximo, os oito anos de idade, ao final do 3º ano do ensino fundamental.
</p>
<p>
    <strong>CLÁUSULA SEGUNDA - DAS RESPONSABILIDADES DO MINISTÉRIO DA EDUCAÇÃO</strong>
</p>
<p>
    O Ministério da Educação, em regime de colaboração com os entes federados, reafirma seu comprometimento e responsabilidade pela alfabetização e o letramento de
    todas as crianças na idade adequada. Para tal, assume a responsabilidade por: promover atividades de formação continuada de coordenadores, orientadores de estudo
    e professores atuantes no ciclo de alfabetização, com aproximação entre as ações de formação e a prática docente; fortalecer e responsabilizar as estruturas estaduais
    de gestão regional da educação básica, intensificando a cooperação entre estados e municípios na coordenação das ações do programa; envolver as gestões estaduais e
    municipais no monitoramento das ações do programa e na avaliação periódica dos resultados dos estudantes; apoiar o desenvolvimento e a impressão de material de formação
    e apoio à prática docente, com foco na aprendizagem do aluno, por iniciativa de estados em parceria com instituições públicas de ensino superior; promover o reconhecimento
    e a valorização das escolas e profissionais comprometidos com a evolução na alfabetização dos alunos das redes de ensino; e envidar esforços para apoiar as escolas com
    maior dificuldade na alfabetização e letramento de alunos no ensino fundamental. Tais responsabilidades se consolidam a partir do compromisso com as seguintes ações, dentre outras:
</p>
<p>
    I - Fomentar ações de mobilização e de gestão do Programa, com a indução da criação de instâncias regionais de gestão compartilhada (estado e municípios),
    responsáveis pelo monitoramento das ações do programa e pela avaliação periódica dos resultados dos estudantes;
</p>
<p>
    II - Promover, em parceria com os sistemas públicos de ensino dos Estados, Distrito Federal e Municípios e as instituições de ensino superior, a formação dos
    coordenadores das ações do Programa nos estados, no Distrito Federal e nos municípios, dos orientadores de estudo e dos professores cursistas nas redes de
    ensino que aderirem às ações do Programa, inclusive com o fomento à formulação e difusão de material de apoio à formação docente, com foco na aprendizagem do aluno;
</p>
<p>
    III - Conceder bolsas de apoio para incentivar a participação dos orientadores de estudo, professores cursistas e coordenadores das Redes nas atividades de
    formação nas redes de ensino que aderirem às ações do Programa;
</p>
<p>
    IV - Realizar avaliações anuais universais, pelo Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira - INEP.
</p>
<p>
    <strong>CLÁUSULA TERCEIRA - DAS RESPONSABILIDADES DA SECÇÃO ESTADUAL DA UNDIME</strong>
</p>
<p>
    O Município confirma os compromissos assumidos pela União e reforça o pacto federativo em torno alfabetização de todas as crianças até,
    no máximo, os oito anos de idade, ao final do 3º ano do ensino fundamental, por meio de ações e responsabilidades que permitam: intensificar
    a cooperação entre estados e municípios na coordenação das ações do programa; apoiar a implementação dos cursos de formação continuada
    nas redes municipais de ensino; colaborar com o monitoramento das ações do programa e a avaliação periódica dos resultados dos estudantes;
    promover o reconhecimento e a valorização das escolas e profissionais comprometidos com a evolução na alfabetização dos alunos das redes de ensino;
    e envidar esforços para as redes municipais de ensino apoiarem as escolas com maior dificuldade na alfabetização e letramento de alunos no ensino fundamental.
    Tais responsabilidades se consolidam a partir do compromisso com as seguintes ações, dentre outras:
</p>
<p>
    I - Ser representado pela UNDIME, que comporá o Comitê Gestor em cada estado, com o intuito de viabilizar o desenvolvimento do Programa;
</p>
<p>
    II - Participar de instâncias regionais de gestão compartilhada (estado e municípios), responsáveis pelo monitoramento das ações do
    programa e pela avaliação periódica dos resultados dos estudantes;
</p>
<p>
    III - Participar das definições em torno das diretrizes pedagógicas vinculadas às políticas de formação continuada de professores com foco na
    alfabetização e letramento, em parceria com os estados, as universidades, ou consórcio de universidades e centros de formação, responsáveis
    pela formação no estado (rede municipal e estadual);
</p>
<p>
    IV - garantir a participação dos orientadores de estudo e dos professores cursistas nas
    atividades de formação, sem prejuízo da carga-horária em sala de aula, custeando o deslocamento e a hospedagem, sempre que necessário;
</p>
<p>
    V - firmar o compromisso de buscar uma evolução na alfabetização dos alunos das redes de ensino considerando a Avaliação Nacional de Alfabetização,
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
    Secretário(a) de Educação do Município de <?php echo $inunome?>
</p>