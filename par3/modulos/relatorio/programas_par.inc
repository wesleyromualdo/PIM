<?php
$strSQL = <<<DML
SELECT
    vTable.estuf,
    vTable.mundescricao,
    vTable.inudescricao,
    vTable.entnome,
    vTable.entemail,
    vTable.enttelefonecomercial,
    vTable.enttelefonecelular,
    vTable.itrid,
    vTable.tendsc
FROM (
    -- Secretário/Dirigente
    (
        (
            SELECT DISTINCT
                instr.estuf, municipio.mundescricao, instr.inudescricao,
                unient.entnome, unient.entemail, unient.enttelefonecomercial, unient.enttelefonecelular,
                CASE WHEN (instr.itrid = 2) THEN 'Municipal' ELSE 'Estadual' END AS itrid,
                tipo.tendsc
            FROM par3.instrumentounidade instr
            JOIN par3.instrumentounidade_entidade     unient ON (unient.inuid = instr.inuid)
            JOIN par3.tipoentidade                      tipo ON (tipo.tenid = unient.tenid)
            LEFT JOIN territorios.municipio        municipio ON (municipio.muncod = instr.muncod)
            WHERE instr.itrid = 2 AND instr.inustatus = 'A' AND unient.tenid IN (4, 9)
        )
        UNION ALL
        (
            SELECT DISTINCT
                instr.estuf, municipio.mundescricao, instr.inudescricao,
                unient.entnome, unient.entemail, unient.enttelefonecomercial, unient.enttelefonecelular,
                CASE WHEN (instr.itrid = 3) THEN 'Estadual' ELSE 'Municipal' END AS itrid,
                tipo.tendsc
            FROM par3.instrumentounidade instr
            JOIN par3.instrumentounidade_entidade  unient ON (unient.inuid = instr.inuid)
            JOIN par3.tipoentidade                   tipo ON (tipo.tenid = unient.tenid)
            LEFT JOIN territorios.municipio     municipio ON (municipio.muncod = instr.muncod)
            WHERE instr.itrid = 3 AND instr.inustatus = 'A' AND unient.tenid IN (4, 9)
        )
    )
    UNION ALL
    -- Nutricionaistas
    (
        (
            SELECT
                ine.estuf, municipio.mundescricao, ine.inudescricao,
                int.entnome, int.entemail, int.enttelefonecomercial, int.enttelefonecelular,
                CASE WHEN (ine.itrid = 2) THEN 'Municipal' ELSE 'Estadual' END AS itrid,
                tipo.tendsc
            FROM par3.instrumentounidade_entidade  int
            INNER JOIN seguranca.usuario             usu ON int.entcpf = usu.usucpf
            INNER JOIN par3.instrumentounidade       ine ON int.inuid = ine.inuid
            INNER JOIN par3.vinculacaonutricionista  vn  ON vn.tenid = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND vn.vnstatus = 'A'
            INNER JOIN par3.situacaonutricionista    sn  ON sn.snid = vn.snid
            JOIN par3.tipoentidade                  tipo ON (tipo.tenid = int.tenid)
            LEFT JOIN territorios.municipio    municipio ON (municipio.muncod = ine.muncod)
            WHERE
               int.entstatus = 'A' AND ine.itrid = 2 AND int.tenid IN (7, 8)
        )
        UNION ALL
        (
            SELECT
                ine.estuf, municipio.mundescricao, ine.inudescricao,
                int.entnome, int.entemail, int.enttelefonecomercial, int.enttelefonecelular,
                CASE WHEN (ine.itrid = 2) THEN 'Municipal' ELSE 'Estadual' END AS itrid,
                tipo.tendsc
            FROM par3.instrumentounidade_entidade  int
            INNER JOIN seguranca.usuario             usu ON int.entcpf = usu.usucpf
            INNER JOIN par3.instrumentounidade       ine ON int.inuid = ine.inuid
            INNER JOIN par3.vinculacaonutricionista  vn  ON vn.tenid = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND vn.vnstatus = 'A'
            INNER JOIN par3.situacaonutricionista    sn  ON sn.snid = vn.snid
            join par3.tipoentidade                  tipo ON (tipo.tenid = int.tenid)
            left join territorios.municipio    municipio ON (municipio.muncod = ine.muncod)
            WHERE
               int.entstatus = 'A' AND ine.itrid = 3 AND int.tenid IN (7, 8)
        )
    )
    UNION ALL
    -- conselheiros
    (
        (
            SELECT DISTINCT
                inu.estuf, municipio.mundescricao, inu.inudescricao,
                ent.entnome, ent.entemail, ent.enttelefonecomercial, ent.enttelefonecelular,
                CASE WHEN (inu.itrid = 2) THEN 'Municipal' ELSE 'Estadual' END AS itrid,
                tipo.tendsc
            FROM par3.instrumentounidade_entidade  ent
            INNER JOIN par3.instrumentounidade     inu ON inu.inuid  = ent.inuid
            JOIN par3.tipoentidade                tipo ON (tipo.tenid = ent.tenid)
            LEFT JOIN territorios.municipio  municipio ON (municipio.muncod = inu.muncod)
            LEFT JOIN (SELECT caeid, entid, MAX(arqid) AS arqid
                    FROM par3.cae_conselheiro conselheiro GROUP BY caeid, entid, arqid
                ) AS sub ON (sub.entid = ent.entid)
            WHERE
                ent.tenid in (13, 14)
                AND ent.entstatus = 'A'
                AND inu.itrid = 2
        )
        UNION ALL
        (
            SELECT DISTINCT
                inu.estuf, municipio.mundescricao, inu.inudescricao,
                ent.entnome, ent.entemail, ent.enttelefonecomercial, ent.enttelefonecelular,
                CASE WHEN (inu.itrid = 2) THEN 'Municipal' ELSE 'Estadual' END AS itrid,
                tipo.tendsc
            FROM par3.instrumentounidade_entidade   ent
            INNER JOIN par3.instrumentounidade      inu ON inu.inuid  = ent.inuid
            JOIN par3.tipoentidade                 tipo ON (tipo.tenid = ent.tenid)
            LEFT JOIN territorios.municipio   municipio ON (municipio.muncod = inu.muncod)
            LEFT JOIN (SELECT caeid, entid, MAX(arqid) AS arqid
                    FROM par3.cae_conselheiro conselheiro GROUP BY caeid, entid, arqid
                ) AS sub ON (sub.entid = ent.entid)
            WHERE
                ent.tenid in (13, 14)
                AND ent.entstatus = 'A'
                AND inu.itrid = 3
        )
    )
) AS vTable
DML;

$cabecalho = ['UF', 'Municípios', 'Descrição', 'Nome', 'E-mail', 'Telefone Comercial', 'Telefone Celular', 'Categoria', 'Cargo/Função'];

if ($_POST['requisicao'] == 'xls') {
    $list = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    $list->setCabecalho($cabecalho);
    $list->setQuery($strSQL);
    $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $list->turnOnOrdenacao('estuf');
    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    exit;
}

require APPRAIZ . 'includes/cabecalho.inc';
?>
<style>
    .fa-file-excel-o{
        font-size:15px;
        color: #fff;
        cursor: pointer;
    }
    .fa-file-excel-o:hover{
        color: #fff;
        -webkit-box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.5);
        -moz-box-shadow:    0px 0px 10px 0px rgba(0,0,0,0.5);
        box-shadow:         0px 0px 10px 0px rgba(0,0,0,0.5);
    }
</style>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-10" >
                    <h3 id="entidade">Relatório de Programa PAR</h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-8 text-center"></div>
                <div class="col-lg-4 text-center"></div>
            </div>
        </div>
        <div class="ibox-footer">
            <div class="row">
                <div class="col-lg-12 text-center">
                    <button type="submit" class="btn btn-success salvar" ><i class="fa fa-search"></i> Filtrar</button>
                    <button type="submit" class="btn btn-info xls" ><i class="fa fa-file-excel-o"></i> XLS</button>
                </div>
            </div>
        </div>
    </div>
</form>
<div class="ibox-content" style="overflow: outo;">
<?php
$list = new Simec_Listagem();
$list->setCabecalho($cabecalho);
$list->setQuery($strSQL);
$list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$list->turnOnOrdenacao('estuf');
$list->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

?>
</div>
<script type="text/javascript">
$(function(){
    $(".xls").on("click", function(){
        $("#requisicao").val("xls");
        $("#formulario").submit();
    });
});
</script>