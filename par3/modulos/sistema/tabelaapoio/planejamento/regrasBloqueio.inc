<?php

switch ($_REQUEST['requisicao']) {
    case 'salvar_bloqueio':
        foreach ($_REQUEST['rebid'] as $rebid) {
            $rebplanejamento = ($_REQUEST['rebplanejamento_'.$rebid] == 'S' ? 't' : 'f');
            $rebempenho = ($_REQUEST['rebempenho_'.$rebid] == 'S' ? 't' : 'f');
            $rebtermo = ($_REQUEST['rebtermo_'.$rebid] == 'S' ? 't' : 'f');
            $rebpagamento = ($_REQUEST['rebpagamento_'.$rebid] == 'S' ? 't' : 'f');
            
            $sql = "UPDATE par3.regra_bloqueio SET rebplanejamento = '{$rebplanejamento}', rebempenho = '{$rebempenho}', rebtermo = '{$rebtermo}', rebpagamento = '{$rebpagamento}' where rebid = {$rebid}";
            $db->executar($sql);
        }
        
        if ($db->commit()) {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'success');
        } else {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'error');
        }
        break;
    case 'salvar_iniciativa':
        foreach ($_REQUEST['repid'] as $repid) {
            $repplanejamento = ($_REQUEST['repplanejamento_'.$repid] == 'S' ? 't' : 'f');
            $repempenho = ($_REQUEST['repempenho_'.$repid] == 'S' ? 't' : 'f');
            $reptermo = ($_REQUEST['reptermo_'.$repid] == 'S' ? 't' : 'f');
            $reppagamento = ($_REQUEST['reppagamento_'.$repid] == 'S' ? 't' : 'f');
            
            $sql = "UPDATE par3.regra_pendecia SET repplanejamento = '{$repplanejamento}', repempenho = '{$repempenho}', reptermo = '{$reptermo}', reppagamento = '{$reppagamento}' where repid = {$repid}";
            $db->executar($sql);
        }
        
        if ($db->commit()) {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'success');
        } else {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'error');
        }
        break;
    case 'salvar_entidade':
        foreach ($_REQUEST['repid'] as $repid) {
            $repplanejamento = ($_REQUEST['repplanejamento_'.$repid] == 'S' ? 't' : 'f');
            $repempenho = ($_REQUEST['repempenho_'.$repid] == 'S' ? 't' : 'f');
            $reptermo = ($_REQUEST['reptermo_'.$repid] == 'S' ? 't' : 'f');
            $reppagamento = ($_REQUEST['reppagamento_'.$repid] == 'S' ? 't' : 'f');
            
            $sql = "UPDATE par3.regra_pendecia SET repplanejamento = '{$repplanejamento}', repempenho = '{$repempenho}', reptermo = '{$reptermo}', reppagamento = '{$reppagamento}' where repid = {$repid}";
            $db->executar($sql);
        }
        
        if ($db->commit()) {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'success');
        } else {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'error');
        }
        break;
    case 'salvar_judicial':
        foreach ($_REQUEST['repid'] as $repid) {
            $repplanejamento = ($_REQUEST['repplanejamento_'.$repid] == 'S' ? 't' : 'f');
            $repempenho = ($_REQUEST['repempenho_'.$repid] == 'S' ? 't' : 'f');
            $reptermo = ($_REQUEST['reptermo_'.$repid] == 'S' ? 't' : 'f');
            $reppagamento = ($_REQUEST['reppagamento_'.$repid] == 'S' ? 't' : 'f');
            
            $sql = "UPDATE par3.regra_pendecia SET repplanejamento = '{$repplanejamento}', repempenho = '{$repempenho}', reptermo = '{$reptermo}', reppagamento = '{$reppagamento}' where repid = {$repid}";
            $db->executar($sql);
        }
        
        if ($db->commit()) {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'success');
        } else {
            simec_redirecionar('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=regrasBloqueio', 'error');
        }
        break;
}

require APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');
?>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Regras de Bloqueios</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-regra-bloqueio" id="form-regra-bloqueio" class="form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value=""/>
                <div class="row">
                    <div class="col-lg-12">
                        <?php
                        $sql = "SELECT b.rebid, p.tipdsc, b.rebplanejamento, b.rebempenho, b.rebtermo, b.rebpagamento FROM par3.regra_bloqueio b
                                	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
                                WHERE p.tipstatus = 'A'
                                ORDER BY b.rebid";
                        $arrBloqueio = $db->carregar($sql);
                        ?>
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="0">
                        <thead>
                            <tr>
                                <th style="text-align: center;">Codigo</th>
                                <th style="text-align: center;">Descrição</th>
                                <th style="text-align: center;">Planejamento</th>
                                <th style="text-align: center;">Empenho</th>
                                <th style="text-align: center;">Termo</th>
                                <th style="text-align: center;">Pagamento</th>
                            </tr>
                        </thead>
                        <tbody>
                        <?php
                        foreach ($arrBloqueio as $v) {
                            echo '<tr>
                        			<td style="text-align: center;">'.$v['rebid'].'</td>
                        			<td style="text-align: center;">'.$v['tipdsc'].'</td>
                        			<td style="text-align: center;">
                                        <input type="hidden" name="rebid[]" id="rebid" value="'.$v['rebid'].'"/>
                                        <div class="checkbox checkbox-success">
                                            <input id="rebplanejamento_'.$v['rebid'].'" name="rebplanejamento_'.$v['rebid'].'" type="checkbox" '.(($v['rebplanejamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="rebplanejamento_'.$v['rebid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="rebempenho_'.$v['rebid'].'" name="rebempenho_'.$v['rebid'].'" type="checkbox" '.(($v['rebempenho'] == 't') ? 'checked' : '').' value="S">
                                            <label for="rebempenho_'.$v['rebid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="rebtermo_'.$v['rebid'].'" name="rebtermo_'.$v['rebid'].'" type="checkbox" '.(($v['rebtermo'] == 't') ? 'checked' : '').' value="S">
                                            <label for="rebtermo_'.$v['rebid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="rebpagamento_'.$v['rebid'].'" name="rebpagamento_'.$v['rebid'].'" type="checkbox" '.(($v['rebpagamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="rebpagamento_'.$v['rebid'].'">&nbsp; </label>
                                        </div></td>
                        		</tr>';
                        }
                        ?>
                        </tbody></table>
                        <div class="ibox-footer">
                            <div class="center">
                                <button type="button" id="btn-salvar-bloqueio" data-dismiss="modal" class="btn btn-primary btn-salvar-bloqueio" data-loading-text="Salvando, aguarde ..."><i class="fa fa-plus"></i> Salvar</button>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Desbloqueio por Iniciativa</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-regra-iniciativa" id="form-regra-iniciativa" class="form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value=""/>
                <div class="row">
                    <div class="col-lg-12">
                        <?php
                        $sql = "SELECT b.repid, p.tipid, p.tipdsc, b.repplanejamento, b.repempenho, b.reptermo, b.reppagamento
                                FROM par3.regra_pendecia b
                                	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
                                	INNER JOIN par3.tipos_desbloqueio t ON t.tdeid = b.tdeid
                                WHERE p.tipstatus = 'A' AND b.tdeid = 1
                                ORDER BY b.repid";
                        $arrBloqueioIniciativa = $db->carregar($sql);
                        ?>
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="0">
                        <thead>
                            <tr>
                                <th style="text-align: center;">Codigo</th>
                                <th style="text-align: center;">Descrição</th>
                                <th style="text-align: center;">Planejamento</th>
                                <th style="text-align: center;">Empenho</th>
                                <th style="text-align: center;">Termo</th>
                                <th style="text-align: center;">Pagamento</th>
                            </tr>
                        </thead>
                        <tbody>
                        <?php
                        foreach ($arrBloqueioIniciativa as $v) {
                            echo '<tr>
                        			<td style="text-align: center;">'.$v['tipid'].'</td>
                        			<td style="text-align: center;">'.$v['tipdsc'].'</td>
                        			<td style="text-align: center;">
                                        <input type="hidden" name="repid[]" id="repid" value="'.$v['repid'].'"/>
                                        <div class="checkbox checkbox-success">
                                            <input id="repplanejamento_'.$v['repid'].'" name="repplanejamento_'.$v['repid'].'" type="checkbox" '.(($v['repplanejamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="repplanejamento_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="repempenho_'.$v['repid'].'" name="repempenho_'.$v['repid'].'" type="checkbox" '.(($v['repempenho'] == 't') ? 'checked' : '').' value="S">
                                            <label for="repempenho_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="reptermo_'.$v['repid'].'" name="reptermo_'.$v['repid'].'" type="checkbox" '.(($v['reptermo'] == 't') ? 'checked' : '').' value="S">
                                            <label for="reptermo_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="reppagamento_'.$v['repid'].'" name="reppagamento_'.$v['repid'].'" type="checkbox" '.(($v['reppagamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="reppagamento_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        		</tr>';
                        }
                        ?>
                        </tbody></table>
                        <div class="ibox-footer">
                            <div class="center">
                                <button type="button" id="btn-salvar-iniciativa" data-dismiss="modal" class="btn btn-primary btn-salvar-iniciativa" data-loading-text="Salvando, aguarde ..."><i class="fa fa-plus"></i> Salvar</button>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Desbloqueio por Entidade</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-regra-entidade" id="form-regra-entidade" class="form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value=""/>
                <div class="row">
                    <div class="col-lg-12">
                        <?php
                        $sql = "SELECT b.repid, p.tipid, p.tipdsc, b.repplanejamento, b.repempenho, b.reptermo, b.reppagamento
                                FROM par3.regra_pendecia b
                                	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
                                	INNER JOIN par3.tipos_desbloqueio t ON t.tdeid = b.tdeid
                                WHERE p.tipstatus = 'A' AND b.tdeid = 2
                                ORDER BY b.repid";
                        $arrBloqueioEntidade = $db->carregar($sql);
                        ?>
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="0">
                        <thead>
                            <tr>
                                <th style="text-align: center;">Codigo</th>
                                <th style="text-align: center;">Descrição</th>
                                <th style="text-align: center;">Planejamento</th>
                                <th style="text-align: center;">Empenho</th>
                                <th style="text-align: center;">Termo</th>
                                <th style="text-align: center;">Pagamento</th>
                            </tr>
                        </thead>
                        <tbody>
                        <?php
                        foreach ($arrBloqueioEntidade as $v) {
                            echo '<tr>
                        			<td style="text-align: center;">'.$v['tipid'].'</td>
                        			<td style="text-align: center;">'.$v['tipdsc'].'</td>
                        			<td style="text-align: center;">
                                        <input type="hidden" name="repid[]" id="repid" value="'.$v['repid'].'"/>
                                        <div class="checkbox checkbox-success">
                                            <input id="repplanejamento_'.$v['repid'].'" name="repplanejamento_'.$v['repid'].'" type="checkbox" '.(($v['repplanejamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="repplanejamento_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="repempenho_'.$v['repid'].'" name="repempenho_'.$v['repid'].'" type="checkbox" '.(($v['repempenho'] == 't') ? 'checked' : '').' value="S">
                                            <label for="repempenho_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="reptermo_'.$v['repid'].'" name="reptermo_'.$v['repid'].'" type="checkbox" '.(($v['reptermo'] == 't') ? 'checked' : '').' value="S">
                                            <label for="reptermo_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="reppagamento_'.$v['repid'].'" name="reppagamento_'.$v['repid'].'" type="checkbox" '.(($v['reppagamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="reppagamento_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        		</tr>';
                        }
                        ?>
                        </tbody></table>
                        <div class="ibox-footer">
                            <div class="center">
                                <button type="button" id="btn-salvar-entidade" data-dismiss="modal" class="btn btn-primary btn-salvar-entidade" data-loading-text="Salvando, aguarde ..."><i class="fa fa-plus"></i> Salvar</button>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Desbloqueio Judicial</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-regra-judicial" id="form-regra-judicial" class="form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value=""/>
                <div class="row">
                    <div class="col-lg-12">
                        <?php
                        $sql = "SELECT b.repid, p.tipid, p.tipdsc, b.repplanejamento, b.repempenho, b.reptermo, b.reppagamento
                                FROM par3.regra_pendecia b
                                	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
                                	INNER JOIN par3.tipos_desbloqueio t ON t.tdeid = b.tdeid
                                WHERE p.tipstatus = 'A' AND b.tdeid = 3
                                ORDER BY b.repid";
                        $arrBloqueioEntidade = $db->carregar($sql);
                        ?>
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="0">
                        <thead>
                            <tr>
                                <th style="text-align: center;">Codigo</th>
                                <th style="text-align: center;">Descrição</th>
                                <th style="text-align: center;">Planejamento</th>
                                <th style="text-align: center;">Empenho</th>
                                <th style="text-align: center;">Termo</th>
                                <th style="text-align: center;">Pagamento</th>
                            </tr>
                        </thead>
                        <tbody>
                        <?php
                        foreach ($arrBloqueioEntidade as $v) {
                            echo '<tr>
                        			<td style="text-align: center;">'.$v['tipid'].'</td>
                        			<td style="text-align: center;">'.$v['tipdsc'].'</td>
                        			<td style="text-align: center;">
                                        <input type="hidden" name="repid[]" id="repid" value="'.$v['repid'].'"/>
                                        <div class="checkbox checkbox-success">
                                            <input id="repplanejamento_'.$v['repid'].'" name="repplanejamento_'.$v['repid'].'" type="checkbox" '.(($v['repplanejamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="repplanejamento_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="repempenho_'.$v['repid'].'" name="repempenho_'.$v['repid'].'" type="checkbox" '.(($v['repempenho'] == 't') ? 'checked' : '').' value="S">
                                            <label for="repempenho_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="reptermo_'.$v['repid'].'" name="reptermo_'.$v['repid'].'" type="checkbox" '.(($v['reptermo'] == 't') ? 'checked' : '').' value="S">
                                            <label for="reptermo_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        			<td style="text-align: center;"><div class="checkbox checkbox-success">
                                            <input id="reppagamento_'.$v['repid'].'" name="reppagamento_'.$v['repid'].'" type="checkbox" '.(($v['reppagamento'] == 't') ? 'checked' : '').' value="S">
                                            <label for="reppagamento_'.$v['repid'].'">&nbsp; </label>
                                        </div></td>
                        		</tr>';
                        }
                        ?>
                        </tbody></table>
                        <div class="ibox-footer">
                            <div class="center">
                                <button type="button" id="btn-salvar-judicial" data-dismiss="modal" class="btn btn-primary btn-salvar-judicial" data-loading-text="Salvando, aguarde ..."><i class="fa fa-plus"></i> Salvar</button>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>
        
       <!--
        <div class="float-e-margins" id="listagem-obras">
            <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                <div class="table-responsive" >
                    <div>
                        <div id="debug"></div>
                        <div id="testeSQL">
                            
                        </div>
                    </div>
                </div>
                <br>
                <br>
            </div>
        </div>
        
    </div>
</div>-->

<script>

$(".btn-salvar-bloqueio").click(function () {
    $btn = $(this).button('loading');
    
    $('[name="requisicao"]').val('salvar_bloqueio');
    $('[name="form-regra-bloqueio"]').submit();
});

$(".btn-salvar-iniciativa").click(function () {
    $btn = $(this).button('loading');
    
    $('[name="requisicao"]').val('salvar_iniciativa');
    $('[name="form-regra-iniciativa"]').submit();
});

$(".btn-salvar-entidade").click(function () {
    $btn = $(this).button('loading');
    
    $('[name="requisicao"]').val('salvar_entidade');
    $('[name="form-regra-entidade"]').submit();
});
$(".btn-salvar-judicial").click(function () {
    $btn = $(this).button('loading');
    
    $('[name="requisicao"]').val('salvar_judicial');
    $('[name="form-regra-judicial"]').submit();
});

    function desativaVinculoRegra(pepid){
        var caminho = window.location.href;
        var action = '&requisicao=desativaVinculoRegra&pepid=' + pepid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                swal({
                        title: "",
                        text: "Regras de Pendências desativada com sucesso",
                        type: "success",
                        html: true,
                        confirmButtonText: "OK",
                        closeOnConfirm: false
                    }, function (isConfirm) {
                        if (isConfirm) {
                            window.location.href = window.location;
                        }
                        return false;
                    }
                );
            }
        });
    }
</script>