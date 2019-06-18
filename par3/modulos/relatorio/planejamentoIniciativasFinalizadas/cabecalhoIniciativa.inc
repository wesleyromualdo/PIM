<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 10/31/2017
 * Time: 5:24 PM
 */
?>
<style>
   .ulClass ul {
        padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;
    }
    .ulClass ul li { display: inline;  text-decoration: none; margin-right: 30px;}

</style>


    <ul style="padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;">
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Dimensão: </strong><?php echo $dbCabecalho['dimesoespadraopar']; ?></li>
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Ciclo: </strong> <?php echo $dbCabecalho['cicdsc'];?></li>
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Anos: </strong> <?php echo str_replace(array("{","}"),"",$dbCabecalho['anos']);?></li>
    </ul>


<ul style="padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;">
    <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Programa: </strong> <?php echo str_replace(array("{","}","\""),null,$dbCabecalho['programas']);?></li>
    </ul>

<ul style="padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;">
    <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Tipo Objeto: </strong> <?php echo $dbCabecalho['intodsc'];?></li>
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Tipo Andamento: </strong> <?php echo $dbCabecalho['intadsc'];?></li>
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Esfera: </strong> <?php echo $dbCabecalho['esfera'];?></li>
    </ul>

<ul style="padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;">
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Áreas Relacionadas:</strong> <?php echo str_replace(array("{","}","\""),null,$dbCabecalho['areasrelacionadas']);?></li>
    </ul>

<ul style="padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;">
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Projeto:</strong> <?php echo str_replace(array("{","}","\""),null,$dbCabecalho['projetos']);?></li>
    </ul>

<ul style="padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;">
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Nível: </strong> <?php echo $dbCabecalho['nivdsc'];?></li>
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Etapa: </strong> <?php echo $dbCabecalho['etadsc'];?></li>
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Modalidade: </strong> <?php echo $dbCabecalho['moddsc'];?></li>
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong>Desdobramento: </strong> <?php echo str_replace(array("{","}","\""),null,$dbCabecalho['des']);?></li>
    </ul>
<ul style="padding:5px;
        margin:0px;
        list-style:none;
        text-decoration: none;">
        <li style="display: inline;  text-decoration: none; margin-right: 30px;"><strong style="color:mediumblue;"> Iniciativa: </strong> <?php echo $dbCabecalho['inddsc'];?></li>
    </ul>
