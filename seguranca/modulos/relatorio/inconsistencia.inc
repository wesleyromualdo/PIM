<?php

include APPRAIZ ."includes/cabecalho.inc";
include APPRAIZ ."zimec/library/Simec/legacy/Grafico.php";

monta_titulo( 'Usuários Incosistentes','' );
?>
<script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/jquery/jquery-isloading.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/jquery/jquery.simple-color.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<!-- Bootstrap JS -->
<script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
<form id="frm" name="frm" method="post">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" valign="top" width="20%">
                    Classificação:
            </td>
            <td>
                <div>
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-raty btn-default <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('C', $_POST['inconsistencia']) ? 'active' : null : null ?>">
                            <input id="inconsistencia"  name="inconsistencia[]" class="raty" type="checkbox" value="C" <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('C', $_POST['inconsistencia']) ? 'checked="checked"' : null : null ?>> Casou
                        </label>
                        <label class="btn btn-raty btn-default <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('S', $_POST['inconsistencia']) ? 'active' : null : null ?>">
                            <input id="inconsistencia"  name="inconsistencia[]" class="raty" type="checkbox" value="S" <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('S', $_POST['inconsistencia']) ? 'checked="checked"' : null : null ?>> Separou
                        </label>
                        <label class="btn btn-raty btn-default <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('D', $_POST['inconsistencia']) ? 'active' : null : null ?>">
                            <input id="inconsistencia"  name="inconsistencia[]" class="raty" type="checkbox" value="D" <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('D', $_POST['inconsistencia']) ? 'checked="checked"' : null : null ?>> Nomes Diferentes
                        </label>
                        <label class="btn btn-raty btn-default <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('I', $_POST['inconsistencia']) ? 'active' : null : null ?>">
                            <input id="inconsistencia"  name="inconsistencia[]" class="raty" type="checkbox" value="I" <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('I', $_POST['inconsistencia']) ? 'checked="checked"' : null : null ?>> Inválido (Não existe na receita)
                        </label>
                        <label class="btn btn-raty btn-default <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('E', $_POST['inconsistencia']) ? 'active' : null : null ?>">
                            <input id="inconsistencia"  name="inconsistencia[]" class="raty" type="checkbox" value="E" <?php echo isset($_POST['inconsistencia']) ? is_array($_POST['inconsistencia']) && in_array('E', $_POST['inconsistencia']) ? 'checked="checked"' : null : null ?>> Erro de escrita
                        </label>
                    </div>
                    <input type="submit" value="Consultar" name="filtrar">
                </div>
            </td>
        </tr>
    </table>
</form>    
    <table class="tabela">
        <tr>
            <td>
<?php

    if(isset($_REQUEST['inconsistencia'])){
        $inconsistencias = $_REQUEST['inconsistencia'];
        for($i=0;$i<count($inconsistencias);$i++){
            $inconsistenciain = "'".$inconsistencias[$i]."',".$inconsistenciain;
        }
        $inconsistenciain = substr($inconsistenciain,0,-1);
        $where = $where . " AND inconsistencia in ({$inconsistenciain})";
    }
                $sqlgrafico =  <<<HTML
                        select 
                                        count(*) as valor,
                                        CASE WHEN inconsistencia = 'E' THEN 
                                                'Erro de escrita'
                                        WHEN  inconsistencia = 'S' THEN
                                                'Separou'
                                        WHEN  inconsistencia = 'C' THEN
                                                'Casou'
                                        WHEN  inconsistencia = 'D' THEN
                                                'Nomes diferentes'
                                        WHEN  inconsistencia = 'I' THEN
                                                'Inválido (Não existe na receita)'
                                        END as descricao
                                    from carga.usuario_inconsistencia
                                    where verificado = 1
                                    and diferente = 1 
                                    and inconsistencia is not null
                                    {$where}
                                        group by inconsistencia
HTML;

                $grafico = new Grafico();
                
?>
                <div>
                    <?php $grafico->setTitulo('Quantidade por Situação')->gerarGrafico($sqlgrafico); ?>
                </div>
            </td>
        </tr>
        <tr>
            <td>
<?php
                $sql = "select distinct
                            '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" style=\"cursor: pointer;\" onclick=\"alterar_usu(' || '''' || usucpf || '''' || ')\">' as acao,                      
                            usucpf,usunome, nomereceita,
                            CASE WHEN inconsistencia = 'E' THEN 
                                    'Erro de escrita'
                            WHEN  inconsistencia = 'S' THEN
                                    'Separou'
                            WHEN  inconsistencia = 'C' THEN
                                    'Casou'
                            WHEN  inconsistencia = 'D' THEN
                                    'Nomes diferentes'
                            WHEN  inconsistencia = 'I' THEN
                                    'Inválido (Não existe na receita)'
                            END as inconsistencia
                        from carga.usuario_inconsistencia
                        where verificado = 1
                        and diferente = 1 
                        and inconsistencia is not null
                        {$where}
                        order by usunome";
                $cabecalho = array("Ações", "CPF", "Usuário (Tabela)", "Usuário (Receita)", "Inconsistência");
                $db->monta_lista( $sql, $cabecalho, 200, 5, 'N', '', '');
                //$db->monta_lista_simples($sql,$cabecalho,10,4550,'N','100%',$par2);
?>
            </td>
        </tr>
    </table>

<script>
    function alterar_usu( usucpf ){
    <?php if ( $_SESSION['sisid'] == 147 ): ?>
            window.open("<?=$_SESSION['sisarquivo']?>.php?modulo=sistema2/usuario2/cadusuario&acao=A&usucpf="+ usucpf+"&voltar=back");
    <?php else: ?>
            window.open("<?=$_SESSION['sisarquivo']?>.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf="+ usucpf+"&voltar=back");
    <?php endif; ?>
    }    
</script>
<link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
<link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>
