<?php
/**
 * Implementação do extrato dinâmico do módulo SpoEmendas.
 *
 * @version $Id: extratodinamico.inc 131128 2017-09-26 12:20:58Z jefersonaraujo $
 */

switch ($_REQUEST['requisicao'])
{
    case 'secretaria':
        global $db;

        $whereSec = ' WHERE 1 = 1 ';
        $whereAcao = ' WHERE 1 = 1 ';
        if (!empty($_REQUEST['unicod']))
        {
            $whereAcao = $whereSec .= ' AND unicod IN (\'' . implode('\',\'', $_REQUEST['unicod']) . '\')';
        }

        if (!empty($_REQUEST['emeano']))
        {
            $whereAcao .= ' AND  prgano IN (\'' . implode('\',\'', $_REQUEST['emeano']) . '\')';
        }

        $sqlAcao = <<<SQL
SELECT DISTINCT acacod AS codigo, acacod || ' - ' || acadsc AS descricao
  FROM monitora.acao
{$whereAcao}
   AND acastatus = 'A'
ORDER BY 2
SQL;
        $sqlSecretaria = <<<SQL
SELECT DISTINCT seccod AS codigo, seccod || ' - ' || secnome AS descricao
  FROM public.secretaria
       LEFT JOIN public.secretariaunidade USING (secid)
{$whereSec}
ORDER BY 2
SQL;
        $json['acacod'] = $db->carregar($sqlAcao);
        if(!$_REQUEST['seccod']) {
            $json['secretaria'] = $db->carregar($sqlSecretaria);
        }
        echo simec_json_encode($json);
        exit;
    case 'unidade':
        global $db;
        if(empty($_REQUEST['seccod'])){
            $sqlUnidade=<<<SQL
					SELECT DISTINCT
                        uni.unicod                      AS codigo,
                        uni.unicod ||' - ' ||uni.unidsc AS descricao
                    FROM
                        public.unidade uni
                    WHERE
                        unistatus = 'A'
                    AND orgcod = '26000'
                    ORDER BY
                        1
SQL;
        }elseif(!empty($_REQUEST['seccod']) && empty($_REQUEST['unicod'])
        || !empty($_REQUEST['seccod']) && !empty($_REQUEST['unicod'])){
            $whereUnidade .= ' AND  pc.seccod IN (\'' . implode('\',\'', $_REQUEST['seccod']) . '\')';
            $sqlUnidade = <<<SQL
            SELECT DISTINCT
                unicod AS codigo,
                unicod ||' - '||unidsc AS descricao
            FROM
                public.unidade pu
            INNER JOIN public.secretariaunidade psu
                    USING(unicod)
            INNER JOIN public.secretaria pc
                    USING(secid)
            WHERE
               	unistatus = 'A'
                AND orgcod = '26000' {$whereUnidade}
SQL;
        }
        if($sqlUnidade) {
            echo simec_json_encode($db->carregar($sqlUnidade));
            exit;
        }
        echo false;
        exit;
}

$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/extratodinamico");

// -- Intercepta requições, principalmente de armazenamento de consultas
$extratoDinamico = new Simec_ExtratoDinamico($_SESSION['sisdiretorio']);
$extratoDinamico->checaRequisicao($fm);

/**
 * Cabeçalho padrão do Simec.
 * @see cabecalho.inc
 */
include APPRAIZ . 'includes/cabecalho.inc';
?>
<div class="col-md-12">
    <?php
    (new Simec_View_Breadcrumb())
        ->add('Extrato Dinâmico')
        ->render();
    echo $fm;
    $extratoDinamico->render();
    ?>
</div>

<div class="modal fade" id="modal-ver-emenda">
    <div class="modal-dialog-large">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhes da Emenda Parlamentar</h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    <?php
    if (!empty($_REQUEST['extrato']['acacod']))
    {
        echo "var valorAcaoPost = ['" . implode('\',\'', $_REQUEST['extrato']['acacod']) . "'];";
    }
    else
    {
        echo "var valorAcaoPost = [];";
    }
    ?>

    $().ready(function () {
        // observa mudanças no select de secretaria, e atualiza o select de ação.
        $('#extrato_unicod').change(populaAcao);

        if($('#extrato_unicod').val() == null) {
            $('#extrato_secretaria').change(populaUnidade);
        }

        // observa mudanças no select de exercício, e atualiza o select de ação.
        $('#extrato_emeano').change(populaAcao);

        if (valorAcaoPost) {
            $('#extrato_acacod').val(valorAcaoPost).trigger('chosen:updated');
            populaAcao();
        }

        $('option[value=""]').prop('disabled', true);
        $('select').trigger('chosen:updated');
    });

    function populaAcao() {
        valorAcaoPost = $('#extrato_acacod').val();

        $.ajax({
            url: "spoemendas.php?modulo=relatorio/extratodinamico&acao=A",
            data: {
                requisicao: "secretaria",
                emeano: $('#extrato_emeano').val(),
                unicod: $('#extrato_unicod').val(),
                seccod: $('#extrato_secretaria').val()
            }
        }).done(function (result) {
            let data = JSON.parse(result);
            // injeta os dados no select de ação.
            for (let tipo in data) {
                let $extratoTipo = $('#extrato_' + tipo);

                // remove options antigos.
                $extratoTipo.find('option').remove();

                $extratoTipo.append($('<option>', {
                        value: '',
                        text: 'Selecione um item',
                        disabled: true
                    })
                );

                $.each(data[tipo], function (index, el) {
                    $extratoTipo.append($('<option>', {
                            value: el.codigo,
                            text: el.descricao
                        })
                    );
                });

                $extratoTipo.trigger('chosen:updated');
            }

            $('#extrato_acacod').val(valorAcaoPost).trigger('chosen:updated');
        });
    }
    function populaUnidade() {
        $.ajax({
            url: "spoemendas.php?modulo=relatorio/extratodinamico&acao=A",
            data: {
                requisicao: "unidade",
                seccod: $('#extrato_secretaria').val(),
                unicod: $('#extrato_unicod').val(),
            }
        }).done(function (result) {
            if(result != false) {
                let data = JSON.parse(result);
                var unicod = $('#extrato_unicod').val();
                $('#extrato_unicod').find('option').remove();
                $.each(data, function (index, el) {
                    $('#extrato_unicod').append($('<option>', {
                            value: el.codigo,
                            text: el.descricao
                        })
                    );
                });
                $('#extrato_unicod').val(unicod);
                $('#extrato_unicod').trigger('chosen:updated');
            }
        });
    }

    function verEmenda(emeid) {
        $('#modal-ver-emenda .modal-body').html('');
        $.ajax({
            url: "spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=ver-emenda&emeid=" + emeid,
        }).done(function (html) {
            conteudo = html;
            $('#modal-ver-emenda .modal-body').html(conteudo);
        });

        $('#modal-ver-emenda').modal();

    }
</script>
