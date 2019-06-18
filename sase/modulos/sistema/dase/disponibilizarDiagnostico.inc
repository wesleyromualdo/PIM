<?php

ob_start();


include APPRAIZ . 'includes/cabecalho.inc';


$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

global $db;

// --

$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = ' Disponibilizar Diagnóstico';


?>
    <link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
    <script type="text/javascript" src="/www/includes/JQuery/jquery-1.10.2.min.js"></script>
    <div id='cabecalhoSistema'>
        <?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
        <center><h3><?=$titulo?></h3></center>
    </div>

    <table width="40%" align="left" border="0" cellspacing="0" cellpadding="2" class="listagem ">
        <thead>
        <tr>
            <td title="Ordenar por Ação" bgcolor=""><strong>Ação</strong></td>
            <td bgcolor=""><strong>Estado</strong></td>

            <td  bgcolor=""><strong>Disponibilizado</strong></td>
            <td  bgcolor=""><strong>Status</strong></td>

        </tr>
        </thead>
        <tbody>
        <?php


             $sql = "SELECT
                        questid,
                        questdescricao,
                        TO_CHAR(datainicio, 'DD/MM/YYYY') as datainicio,
                        TO_CHAR(datafim, 'DD/MM/YYYY') as datafim,
                        queststatus
                    FROM sase.questionariosase
                    ";
             $questnarios = $db->carregar($sql);

        ?>
        <?php foreach ($questnarios as $questnario):?>
         <tr>
            <td align="left" width="10%" title="Ação">
                <a style="cursor:pointer" onclick="editarquest( <?=$questnario['questid']?> )"><img src="/imagens/alterar.gif"></a>
            </td>
            <td align="left"  ><?=$questnario['questdescricao']?></td>
            <td align="left"  ><?="De ".$questnario['datainicio']." Até ".$questnario['datafim'] ?></td>
            <td align="left" ><?=$questnario['queststatus'] == 'I'?'Inativo':'Ativo'?></td>

        </tr>
        <?php endforeach; ?>

        </tbody>
    </table>


    <script>
        /**
         * Manda para edicao de questoespontuais
         */
        function editarquest( questid ){

            if( confirm("Deseja realmente Editar esse Diagnóstico?") )
            {
                window.location.href = '/sase/sase.php?modulo=sistema/dase/editadiagnostico&acao=A&quest=' + questid;
            }
        }


    </script>
<?php




