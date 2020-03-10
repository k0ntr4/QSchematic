#pragma once

#include <QAction>
#include "item.h"
#include "wirepoint.h"
#include "wirenet.h"
#include "line.h"
#include "wire_system/wire.h"

class QVector2D;

namespace QSchematic {

    class Line;

    /**
     * IMPORTANT NOTE: The points coordinates are RELATIVE and in SCENE COORDINATES.
     *                 Wires must be movable so we can move entire groups of stuff.
     */
    class Wire : public Item, public wire_system::wire
    {
        Q_OBJECT

    public:
        Wire(int type = Item::WireType, QGraphicsItem* parent = nullptr);
        virtual ~Wire() override;
        virtual void update() override;

        virtual gpds::container to_container() const override;
        virtual void from_container(const gpds::container& container) override;
        virtual std::shared_ptr<Item> deepCopy() const override;
        virtual QRectF boundingRect() const override;
        virtual QPainterPath shape() const override;

        void prependPoint(const QPointF& point);
        void appendPoint(const QPointF& point);
        void insertPoint(int index, const QPointF& point);        // Index of new point
        void removeFirstPoint();
        void removeLastPoint();
        void removePoint(int index);
        QVector<WirePoint> wirePointsRelative() const;
        QVector<QPointF> pointsRelative() const;
        QVector<QPointF> pointsAbsolute() const;
        void simplify();
        void movePointBy(int index, const QVector2D& moveBy);
        void move_point_to(int index, const QPointF& moveTo) override;
        void moveLineSegmentBy(int index, const QVector2D& moveBy);
        void setPointIsJunction(int index, bool isJunction);
        bool pointIsOnWire(const QPointF& point) const;
        bool connectWire(Wire* wire);
        void disconnectWire(Wire* wire);
        void setNet(const std::shared_ptr<WireNet>& wirenet);
        std::shared_ptr<WireNet> net();
        bool movingWirePoint() const;
        void updatePosition();

    signals:
        void pointMoved(Wire& wire, WirePoint& point);
        void pointMovedByUser(Wire& wire, int index);
        void pointRemoved(int index);
        void toggleLabelRequested();

    protected:
        void copyAttributes(Wire& dest) const;
        void calculateBoundingRect();
        void setRenameAction(QAction* action);

        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
        virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
        virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;

    private:
        Q_DISABLE_COPY(Wire)

        void removeDuplicatePoints();
        void removeObsoletePoints();

        QRectF _rect;
        int _pointToMoveIndex;
        int _lineSegmentToMoveIndex;
        QPointF _prevMousePos;
        QPointF _offset;
        QAction* _renameAction;
        std::shared_ptr<WireNet> _net;
        bool _internalMove;
    };

}
