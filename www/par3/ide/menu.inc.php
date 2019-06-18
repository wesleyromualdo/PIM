<?php
$fileName = pathinfo($_SERVER['SCRIPT_FILENAME'], PATHINFO_FILENAME);

if(empty($ide->muncod)){
	$itemMenu09c = "<li class='tabela'>
			            <a href='#09c' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
			                <span class='nav-label'>Tabela 9-C. Distribuição de Matrículas do Ensino Médio por Modalidade - Rede Estadual do Estado</span>
			            </a>
			        </li>";
}

$menuHtml = "
    <ul class='nav metismenu' id='side-menu' style='font-size: 11px;'>
        <li>
            <a href='index.php' style='padding: 5px 2px 10px 8px;border-bottom: 1px solid #68859E; color: #ffffff;' class='menu'><i class='fa fa-search'></i> <span class='nav-label'>NOVA CONSULTA</span> </a>
        </li>

        <li>
            <input style='width: 100%' type='text' placeholder='filtrar menu' id='textFind' class='input-sm'>
        </li>

		<li class='tabela'>
            <a href='#03' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 3. Índice de Desenvolvimento da Educação Básica</span>
            </a>
        </li>

        <li class='tabela'>
            <a href='#06a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 6-A. Número de Escolas por Etapa de Ensino - Rede Estadual do Estado</span>
            </a>
        </li>

        <li class='tabela'>
            <a href='#06b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 6-B. Número de Escolas por Etapa de Ensino - Redes Municipais</span>
            </a>
        </li>

        <li class='tabela'>
            <a href='#07a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 7-A. Número de Escolas Rurais em Áreas Específicas - Rede Estadual</span>
            </a>
        </li>
         <li class='tabela'>
            <a href='#07b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 7-B. Número de Escolas Rurais em Áreas Específicas - Redes Municipais</span>
            </a>
        </li>
		
        <li class='tabela'>
            <a href='#08a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 8-A. Número de Escolas por Modalidade e Etapa de Ensino - Rede Estadual</span>
            </a>
        </li>
         <li class='tabela'>
            <a href='#08b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 8-B. Número de Escolas por Modalidade e Etapa de Ensino - Rede Municipal</span>
            </a>
        </li>		

        <li class='tabela'>
            <a href='#09a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 9-A. Matrículas por Modalidade, Etapa e Turno - Rede Estadual</span>
            </a>
        </li>
        <li class='tabela'>
            <a href='#09b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 9-B. Matrículas por Modalidade, Etapa e Turno - Rede Municipal</span>
            </a>
        </li>		
		
		$itemMenu09c
		
        <li class='tabela'>
            <a href='#10a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 10-A. Funções Docentes por Modalidade e Etapa de Ensino - Rede Estadual </span>
            </a>
        </li>
         <li class='tabela'>
            <a href='#10b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 10-B. Funções Docentes por Modalidade e Etapa de Ensino - Redes Municipais</span>
            </a>
        </li>		
		
        <li class='tabela'>
            <a href='#11a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 11-A  Condições de Atendimento Diurno - Rede Estadual</span>
            </a>
        </li>
         <li class='tabela'>
            <a href='#11b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 11-B. Condições de Atendimento Diurno - Redes Municipais</span>
            </a>
        </li>
        <li class='tabela'>
            <a href='#12a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 12-A. Condições de Atendimento - Noturno - Rede Estadual</span>
            </a>
        </li>
        <li class='tabela'>
            <a href='#12b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 12-B. Condições de Atendimento - Noturno - Redes Municipais</span>
            </a>
        </li>

        <li class='tabela'>
            <a href='#13a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 13-A. Taxas de Rendimento - Rede Estadual do Estado</span>
            </a>
        </li>
		<li class='tabela'>
            <a href='#13b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 13-B. Taxas de Rendimento - Redes Municipais do Estado</span>
            </a>
        </li>

        <li class='tabela'>
            <a href='#14a' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 14-A. Matrículas em Turmas de Correção de Fluxo - Rede Estadual</span>
            </a>
        </li>
         <li class='tabela'>
            <a href='#14b' style='padding:5px 2px 10px 8px;border-bottom: 1px solid #68859E;' class='menu'>
                <span class='nav-label'>Tabela 14-B. Matrículas em Turmas de Correção de Fluxo - Redes Municipais</span>
            </a>
        </li>
    </ul>";