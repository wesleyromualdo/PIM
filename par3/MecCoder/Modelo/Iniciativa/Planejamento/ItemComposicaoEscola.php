<?php
namespace Simec\Par3\Modelo\Iniciativa\Planejamento;

class ItemComposicaoEscola extends \Simec\AbstractModelo
{

    protected function colunaQuantidadeAlunos($inpid)
    {
        $voIniciativaPlanejamento = new \Simec\Par3\Dado\Iniciativa\Planejamento\Planejamento(['inpid' => $inpid]);
        $this->lerObjeto($voIniciativaPlanejamento);
        return [
            '1' => 'escqtd_alunos_infantil',
            '2' => 'escqtd_alunos_fundamental',
            '3' => 'escqtd_alunos_medio',
            '5' => '(escqtd_alunos_medio+escqtd_alunos_fundamental+escqtd_alunos_infantil)',
            ][$voIniciativaPlanejamento->etaid];
    }

    protected function filtroLocalizacao($inpid)
    {
        $modelDesdobramentos = new Desdobramentos();
        $desdobramentos = $modelDesdobramentos->recuperarDesdobramentos($inpid);

        $res = '1,2';
        if (in_array(\Simec\Par3\Modelo\Iniciativa\Desdobramento\Desdobramento::urbano(), $desdobramentos)) {
            $res = '1';
        }

        if (in_array(\Simec\Par3\Modelo\Iniciativa\Desdobramento\Desdobramento::rural(), $desdobramentos)) {
            $res = '2';
        }

        if (in_array(array(
                \Simec\Par3\Modelo\Iniciativa\Desdobramento\Desdobramento::urbano(),
                \Simec\Par3\Modelo\Iniciativa\Desdobramento\Desdobramento::rural()
                ), $desdobramentos)) {
            $res = '1,2';
        }
        return $res;
    }

    public function listarEscolas($params)
    {
        $colunaQtdAlunos = $this->colunaQuantidadeAlunos($params['inpid']);
        $filtroLocalizacao = sprintf('AND escola.escco_localizacao IN(%s)',$this->filtroLocalizacao($params['inpid']));
        $escoEsfera = $this->lerObjeto(new \Simec\Par3\Dado\Instrumento\InstrumentoUnidade($params))->itrid == 2 ? 'M' : 'E';
        $muncod = $params['muncod'] ? " AND mun.muncod = '{$params['muncod']}' " : '';
        $validarItrid =  "AND escola.escco_esfera = '{$escoEsfera}'";

        $join = '';
        $colQtdEmenda = '';
        if ($params['ineid']) {
            $join = "LEFT JOIN (
				        select
	                    	escola.ipequantidade,
	                        escola.escid,
	                        iee.ieequantidade
				        FROM
				        	par3.iniciativa_emenda ie
				        join par3.iniciativa_planejamento ip on ip.inpid = ie.inpid and
				        										ip.inpstatus = 'A'
				        join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
				        														ic.ipistatus = 'A'
				        join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
				        																   escola.ipestatus = 'A'
				        LEFT JOIN par3.iniciativa_emenda_item_composicao_escola iee ON iee.ipeid = escola.ipeid and
																				       iee.ineid = ie.ineid and
																				       iee.ieestatus = 'A'
				        WHERE
				        	ie.inestatus = 'A'
				        	AND ie.ineid = {$params['ineid']}
				        	AND ic.inpid  = {$params['inpid']}
				        	AND ic.ipiano = {$params['iniano']}
				       		AND ic.iigid  = {$params['iigid']}
	                ) AS dadosescola ON (dadosescola.escid = escola.escid)";
            $colQtdEmenda = "dadosescola.ieequantidade  as qtd_emenda,";
        } else {
            $join = "LEFT JOIN (
				        select
	                    	escola.ipequantidade,
	                        escola.escid
				        FROM
				        	par3.iniciativa_planejamento ip
				        join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
				        														ic.ipistatus = 'A'
				        join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
				        																   escola.ipestatus = 'A'
				        WHERE
				        	ip.inpstatus = 'A'
				        	AND ic.inpid  = {$params['inpid']}
				        	AND ic.ipiano = {$params['iniano']}
				       		AND ic.iigid  = {$params['iigid']}
	                ) AS dadosescola ON (dadosescola.escid = escola.escid)";
        }
        $arrEsc = $this->carregarEscidItemComposicaoEscolas($params);
        $orEscid = '';
        if (!empty($arrEsc)) {
            $arrEsc = array_map(function ($esc) {
                return $esc['escid'];
            }, $arrEsc);
            $arrEscFilter = array_filter($arrEsc);
            if (count($arrEscFilter) > 0) {
                $implodeEsc = implode(',', $arrEscFilter);
                $orEscid = "OR escola.escid IN ({$implodeEsc})";
            }
        }
        $sql = <<<sql
            SELECT DISTINCT ON(escola.escid)
                       mun.mundescricao as "nmMunicipio",
                       escola.escid as "idEscola",
                       escola.escnome as "nmEscola",
                       escola.esccodinep as "coInep",
                       ipequantidade as "quantidade",
                       {$colQtdEmenda}
	                   escola.escno_localizacao as "localizacao",
	                   escola.escco_esfera as "coEsfera",
	                   escola.escno_esfera as "nmEsfera",
                       escqtd_salas_utilizadas as "qtSalas",
                       COALESCE($colunaQtdAlunos,0) as "qtAlunos"
                FROM par3.instrumentounidade ins
                INNER JOIN territorios.municipio mun ON ((ins.muncod IS NOT NULL
                     AND ins.muncod = mun.muncod)
                     OR (ins.muncod IS NULL
                     AND ins.estuf = mun.estuf))
                INNER JOIN par3.escola escola ON (mun.muncod::bigint = escola.muncod::bigint AND (escola.escco_situacao_funcoinamento in (1,2) {$orEscid}))
                {$join}
                WHERE
                    ins.inuid={$params['inuid']}
                    AND COALESCE($colunaQtdAlunos,0) >0
                    AND (
                        (ins.muncod IS NOT NULL)
                        OR (ins.muncod IS NULL)
                    )
                           {$filtroLocalizacao}
                           {$muncod}
                           {$validarItrid}
                ORDER BY escola.escid
sql;
        return $this->carregar($sql);
    }

    /**
     * Retorna array de escolas vinculadas ao planejamento
     * @param $dados
     * @return array
     */
    public function carregarEscidItemComposicaoEscolas($dados)
    {
        $sql = '';
        if ( $dados['ineid'] ) {
            $sql = "
                        select
                            escola.escid
                        FROM
                            par3.iniciativa_emenda ie
                        join par3.iniciativa_planejamento ip on ip.inpid = ie.inpid and
                                                                ip.inpstatus = 'A'
                        join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
                                                                                ic.ipistatus = 'A'
                        join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
                                                                                           escola.ipestatus = 'A'
                        LEFT JOIN par3.iniciativa_emenda_item_composicao_escola iee ON iee.ipeid = escola.ipeid and
                                                                                       iee.ineid = ie.ineid and
                                                                                       iee.ieestatus = 'A'
                        WHERE
                            ie.inestatus = 'A'
                            AND ie.ineid = {$dados['ineid']}
                            AND ic.inpid  = {$dados['inpid']}
                            AND ic.ipiano = {$dados['iniano']}
                            AND ic.iigid  = {$dados['iigid']}
                    ";
        }else {
            $sql = "select
                        escola.escid
                    FROM
                        par3.iniciativa_planejamento ip
                    join par3.iniciativa_planejamento_item_composicao ic on ic.inpid = ip.inpid and
                                                                            ic.ipistatus = 'A'
                    join par3.iniciativa_planejamento_item_composicao_escola escola on escola.ipiid = ic.ipiid and
                                                                                       escola.ipestatus = 'A'
                    WHERE
                        ip.inpstatus = 'A'
                        AND ic.inpid  = {$dados['inpid']}
                        AND ic.ipiano = {$dados['iniano']}
                        AND ic.iigid  = {$dados['iigid']}
            ";
        }
        $arrEsc = $this->carregar($sql);
        if(!$arrEsc) {
            return array();
        }
        return (!empty(array_filter($arrEsc)) ? $arrEsc :array());
    }

    public function listarEscolasSelecionadas()
    {
        return [
            ['nmMunicipio' => 'Brasília', 'nmEscola' => 'CEM 02 DE PLANALTINA', 'coInep' => '53006070', 'localizacao' => 'Urbana', 'nmEsfera' => 'Estadual', 'qtSalas' => 21, 'qtAlunos' => 1698],
            ['nmMunicipio' => 'Brasília', 'nmEscola' => 'CED 03 DO GUARA', 'coInep' => '53008472', 'localizacao' => 'Urbana', 'nmEsfera' => 'Estadual', 'qtSalas' => 27, 'qtAlunos' => 1056],
            ['nmMunicipio' => 'Brasília', 'nmEscola' => 'CED 02 DE SOBRADINHO', 'coInep' => '53005473', 'localizacao' => 'Urbana', 'nmEsfera' => 'Estadual', 'qtSalas' => 19, 'qtAlunos' => 1501],
            ['nmMunicipio' => 'Brasília', 'nmEscola' => 'CED 104 DO RECANTO DAS EMAS', 'coInep' => '53011066', 'localizacao' => 'Urbana', 'nmEsfera' => 'Estadual', 'qtSalas' => 25, 'qtAlunos' => 1651],
            ['nmMunicipio' => 'Brasília', 'nmEscola' => 'CEM AVE BRANCA', 'coInep' => '53003632', 'localizacao' => 'Urbana', 'nmEsfera' => 'Estadual', 'qtSalas' => 28, 'qtAlunos' => 2436],
        ];
    }
}
