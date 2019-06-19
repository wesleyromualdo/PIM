<?php /* ****arquivo vazio**** */



function pegaSolicitacoesObra($obrid)
{
    global $db;

    $sql = "SELECT      sol.slcid,
		                sol.slcobservacao,
		                sol.slcjustificativa,
		                TO_CHAR(sol.slcdatainclusao,'DD/MM/YYYY') as slcdatainclusao,
		                sol.docid,
		                sol.qrpid,
		                sol.aprovado,
		                ts.tslid,
		                ts.tsldescricao,
		                esd.esdid,
		                esd.esddsc,
		                usu.usunome AS criador,
		                qr.queid
            FROM 		obras2.solicitacao sol
            INNER JOIN 	obras2.tiposolicitacao_solicitacao tss ON(sol.slcid = tss.slcid)
            INNER JOIN 	obras2.tiposolicitacao ts ON(tss.tslid = ts.tslid)
            INNER JOIN 	workflow.documento doc ON(sol.docid = doc.docid)
            INNER JOIN 	workflow.estadodocumento esd ON(doc.esdid = esd.esdid)
            INNER JOIN 	seguranca.usuario usu ON(sol.usucpf = usu.usucpf)
            LEFT  JOIN 	questionario.questionarioresposta qr ON(sol.qrpid = qr.qrpid)
            WHERE 		obrid = {$obrid} AND tss.tpsstatus = 'A' AND sol.slcstatus = 'A' AND ts.tslid IN (2,3,4) AND sol.aprovado = 'S'
            ORDER BY 	1 DESC";
    
    $retorno = $db->carregar($sql);
    
    return $retorno ? $retorno : array();
}

function projetoFNDE($obrid){
    $obra = new Obras();
	$obra->carregarPorIdCache($obrid);
    $tpoProjetoFnde = array(
        1, // Espaço Educativo - 01 Sala
        2, // Espaço Educativo - 02 Salas
        3, // Espaço Educativo - 04 Salas
        4, // Espaço Educativo - 06 Salas
        5, // Espaço Educativo - 08 Salas
        6, // Espaço Educativo - 10 Salas
        7, // Espaço Educativo - 12 Salas
        8, // Espaço Educativo Ensino Médio Profissionalizante
        9, // Escola de Educação Infantil Tipo B
        10, // Escola de Educação Infantil Tipo C
//        13, // Escola com projeto elaborado pelo concedente
//        14, // Escola com projeto elaborado pelo concedente
//        15, // Escola com projeto elaborado pelo concedente
//        16, // Escola de Educação Infantil Tipo A
        17, // QUADRA ESCOLAR COBERTA COM PALCO- PROJETO FNDE
//        18, // QUADRA ESCOLAR COBERTA - PROJETO PRÓPRIO
        19, // COBERTURA DE QUADRA ESCOLAR PEQUENA - PROJETO FNDE
        20, // COBERTURA DE QUADRA ESCOLAR GRANDE - PROJETO FNDE
//        21, // COBERTURA DE QUADRA ESCOLAR - PROJETO PRÓPRIO
        22, // QUADRA ESCOLAR COBERTA COM VESTIÁRIO- PROJETO FNDE
//        102, // Reforma
//        103, // Ampliação
        104, // MI - Escola de Educação Infantil Tipo B
        105, // MI - Escola de Educação Infantil Tipo C
        106, // Escola com Projeto elaborado pelo proponente
        107, // Escola 12 salas - Projeto FNDE 2014
        108, // Projeto 1 Convencional
        109, // Projeto 2 Convencional
        110, // Projeto 1 Convencional - Emenda
        111, // Projeto 2 Convencional - Emenda
//        112, // Construção
        113, // Projeto Tipo B - Bloco Estrutural
        114, // Projeto Tipo C - Bloco Estrutural

    );

    if(in_array($obra->tpoid, $tpoProjetoFnde)){
        return true;
    } else {
        return false;
    }
}
