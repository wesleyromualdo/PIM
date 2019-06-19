<?php
class DadoAgendaComissaoCoordenadoraExterno extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados do Mapa  Agenda de Trabalho da Comissão Coordenadora. 
     *
     * @return array cores do mapa e array legenda
     */
    public function carregaDado()
    {
        global $db;
        if (count($this->estuf) > 0) {
            $and = " AND e.estuf IN ('".(implode("','",$this->estuf))."')";
        }
        $sql = <<<DML
                SELECT  DISTINCT
                        CASE WHEN accdatainclusao IS NOT NULL
                            THEN '#006600'
                            ELSE '#FFFFFF'
                        END AS cor,
            			CASE WHEN accdatainclusao IS NOT NULL
                            THEN 'Preenchido'
                            ELSE 'Não preenchido'
                        END AS situacao,
                        e.muncod
                FROM territorios.municipio AS e
                LEFT JOIN sase.agendacomissaocoordenadora AS a ON a.muncod = e.muncod AND a.accstatus = 'A'
                WHERE 1=1
                {$and}
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $estuf = $this->estuf;

        $where = ( ( $estuf!='' && count($estuf)>0 ) ? " and m.estuf in ( '". ( implode( "','", $estuf ) ) ."' ) " : "" );

                        $sql = <<<DML
            SELECT 
                count(DISTINCT m.*) as total,
                'Preenchido' as descricao,
                '#006600' as cor
            FROM 
                territorios.municipio AS m 
            LEFT JOIN sase.agendacomissaocoordenadora AS a ON a.muncod = m.muncod
            WHERE accdatainclusao IS NOT NULL
            {$where}
            UNION ALL
            SELECT count(DISTINCT m.*) as total,
                 'Não preenchido' as descricao,
                 '#FFFFFF' as cor
            FROM territorios.municipio AS m
            LEFT JOIN sase.agendacomissaocoordenadora AS a ON a.muncod = m.muncod
            WHERE accdatainclusao IS NULL
            {$where}
            ORDER BY descricao
DML;
                
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
